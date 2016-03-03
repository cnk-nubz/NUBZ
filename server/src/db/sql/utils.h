#ifndef DB_SQL__UTILS__H
#define DB_SQL__UTILS__H

#include <string>
#include <type_traits>

#include <boost/optional.hpp>

namespace db {
namespace sql {
namespace utils {

template <class FCol, class... Cols>
struct same_table : std::true_type {};

template <class FCol, class SCol, class... Cols>
struct same_table<FCol, SCol, Cols...> {
    static constexpr bool value =
        std::is_same<typename FCol::table_type, typename SCol::table_type>::value &&
        same_table<SCol, Cols...>::value;
};

template <class Col>
std::string getColumnName() {
    return std::decay_t<Col>::name;
}

template <class FirstCol, class... Cols>
std::string getTableName() {
    static_assert(same_table<FirstCol, Cols...>::value,
                  "all columns should be from the same table");
    return FirstCol::table_type::tableName;
}

inline std::string asSqlList(const std::vector<std::string> &list,
                             const std::string &separator = ", ") {
    std::string res;
    auto it = list.cbegin();
    while (it != list.cend()) {
        res += *it;
        if (++it != list.cend()) {
            res += separator;
        }
    }
    return res;
}

inline std::string sqlVal(const bool &raw) {
    return raw ? "TRUE" : "FALSE";
}

inline std::string sqlVal(const std::string &raw) {
    return "'" + raw + "'";
}

inline std::string sqlVal(const boost::optional<std::string> &raw) {
    if (raw) {
        return sqlVal(raw.value());
    } else {
        return "NULL";
    }
}

template <class T>
std::string sqlVal(const boost::optional<T> &raw) {
    if (raw) {
        return std::to_string(raw.value());
    } else {
        return "NULL";
    }
}

template <class T>
std::string sqlVal(const T &raw) {
    return std::to_string(raw);
}
}
}
}

#endif