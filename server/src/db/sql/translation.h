#ifndef DB_SQL__TRANSLATION__H
#define DB_SQL__TRANSLATION__H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include <libpq-fe.h>

namespace db {
namespace sql {
namespace translation {

template <class T>
struct sql_val_cast {
    static T from(const std::string &raw) {
        return T{raw};
    }

    static std::string to(const T &raw) {
        return raw;
    }
};

template <>
struct sql_val_cast<std::int32_t> {
    static std::int32_t from(const std::string &raw) {
        std::int32_t val;
        if (!boost::conversion::try_lexical_convert(raw, val)) {
            assert(false && "unable to convert to int32_t");
        }
        return val;
    }

    static std::string to(std::int32_t raw) {
        return std::to_string(raw);
    }
};

template <>
struct sql_val_cast<std::string> {
    static std::string from(const std::string &raw) {
        return raw;
    }

    static std::string to(const std::string &raw) {
        auto buf = std::make_unique<char[]>(raw.size() * 3);
        PQescapeString(buf.get(), raw.c_str(), raw.size());
        return "'" + std::string{buf.get()} + "'";
    }
};

template <>
struct sql_val_cast<boost::gregorian::date> {
    static boost::gregorian::date from(const std::string &raw) {
        try {
            return boost::gregorian::from_simple_string(raw);
        } catch (...) {
            assert(false && "unable to convert to date");
            return {};
        }
    }

    static std::string to(const boost::gregorian::date &raw) {
        return sql_val_cast<std::string>::to(boost::gregorian::to_simple_string(raw));
    }
};

template <>
struct sql_val_cast<bool> {
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

    static std::string to(bool raw) {
        return raw ? "TRUE" : "FALSE";
    }
};

inline std::string sqlVal(std::int32_t raw) {
    return sql_val_cast<std::int32_t>::to(raw);
}

inline std::string sqlVal(const boost::gregorian::date &raw) {
    return sql_val_cast<boost::gregorian::date>::to(raw);
}

inline std::string sqlVal(const std::string &raw) {
    return sql_val_cast<std::string>::to(raw);
}

inline std::string sqlVal(bool raw) {
    return sql_val_cast<bool>::to(raw);
}

template <class T>
std::string sqlVal(const boost::optional<T> &raw) {
    if (raw) {
        return sqlVal(raw.value());
    } else {
        return "NULL";
    }
}
}
}
}

#endif