#ifndef DB_SQL__UTILS__H
#define DB_SQL__UTILS__H

#include <string>
#include <type_traits>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>

namespace db {
namespace sql {
namespace utils {

template <class FField, class... Fields>
struct same_table : std::true_type {};

template <class FField, class SField, class... Fields>
struct same_table<FField, SField, Fields...> {
    static constexpr bool value =
        std::is_same<typename FField::table_type, typename SField::table_type>::value &&
        same_table<SField, Fields...>::value;
};

template <class Field>
std::string getColumnName() {
    return std::decay_t<Field>::columnName;
}

template <class Field, class... Tail>
std::string getTableName() {
    static_assert(same_table<Field, Tail...>::value, "all columns should be from the same table");
    return Field::table_type::tableName;
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
}
}
}

#endif