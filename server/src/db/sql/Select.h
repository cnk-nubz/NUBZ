#ifndef DB_SQL__SELECT__H
#define DB_SQL__SELECT__H

#include <string>
#include <tuple>
#include <vector>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include <db/table/Value.h>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... Cols>
class Select : public Where<Select<Cols...>, Cols...> {
    template <class Col>
    struct correct_column {
        using v_type = typename Col::value_type;
        static constexpr bool value =
            std::is_base_of<db::table::detail::Value<typename v_type::type>, v_type>::value ||
            std::is_base_of<db::table::detail::OptValue<typename v_type::internal_type>,
                            v_type>::value;
    };

    static_assert(utils::same_table<Cols...>::value, "all columns should be from the same table");
    static_assert(
        ::utils::types::all_of<correct_column<Cols>...>::value,
        "each column's value type should be a subclass of either db::table::detail::Value<> "
        "or OptValue<>");

public:
    using return_type = std::tuple<typename std::decay_t<Cols>::value_type...>;

    std::string str() const {
        boost::format stmt("SELECT %1%\nFROM %2%\n%3%");
        stmt % utils::asSqlList({utils::getColumnName<Cols>()...});
        stmt % utils::getTableName<Cols...>();
        stmt % this->whereStmt();
        return stmt.str();
    }
};
}
}

#endif