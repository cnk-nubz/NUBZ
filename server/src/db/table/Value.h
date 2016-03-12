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

namespace helpers {}  // helpers
}
}

#endif