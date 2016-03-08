#ifndef DB_TABLE__VALUE__H
#define DB_TABLE__VALUE__H

#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <unordered_set>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace db {
namespace table {
namespace detail {

template <class T>
struct Value {
    using type = T;
    using internal_type = T;
    static constexpr bool is_optional = false;

    explicit Value(type v) : value(std::move(v)) {
    }
    const type value;
};

template <class T>
struct OptValue {
    using type = boost::optional<T>;
    using internal_type = T;
    static constexpr bool is_optional = true;

    explicit OptValue(type v) : value(std::move(v)) {
    }
    const type value;
};
}  // detail

namespace helpers {

template <class>
struct value_cast;

template <>
struct value_cast<std::int32_t> {
    static std::int32_t from(const std::string &raw) {
        std::int32_t val;
        if (!boost::conversion::try_lexical_convert(raw, val)) {
            assert(false && "unable to convert to int32_t");
        }
        return val;
    }
};

template <>
struct value_cast<boost::gregorian::date> {
    static boost::gregorian::date from(const std::string &raw) {
        try {
            return boost::gregorian::from_simple_string(raw);
        } catch (...) {
            assert(false && "unable to convert to date");
            return {};
        }
    }
};

template <>
struct value_cast<std::string> {
    static std::string from(const std::string &raw) {
        return raw;
    }
};

template <>
struct value_cast<bool> {
    static bool from(const std::string &raw) {
        static const std::unordered_set<std::string> trueLiteral = {
            "TRUE", "t", "true", "y", "yes", "on", "1"};
        static const std::unordered_set<std::string> falseLiteral = {
            "FALSE", "f", "false", "n", "no", "off", "0"};

        if (trueLiteral.count(raw)) {
            return true;
        }
        if (falseLiteral.count(raw)) {
            return false;
        }

        assert(false);
    }
};
}  // helpers

template <class Value, bool optional = Value::is_optional>
struct ValueFactory;

template <class Value>
struct ValueFactory<Value, true> {
    static Value create(const boost::optional<std::string> &raw) {
        return !raw ? Value{{}} : ValueFactory<Value, false>::create(raw.value());
    };
};

template <class Value>
struct ValueFactory<Value, false> {
    static Value create(const boost::optional<std::string> &raw) {
        assert(raw);
        return create(raw.value());
    };

    static Value create(const std::string &raw) {
        return Value{helpers::value_cast<typename Value::internal_type>::from(raw)};
    };
};
}
}

#endif