#ifndef DB_SQL__SELECT__H
#define DB_SQL__SELECT__H

#include <string>
#include <tuple>
#include <vector>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include "Where.h"
#include "utils.h"

namespace db {
namespace sql {

template <class... Fields>
class Select : public Where<Select<Fields...>, Fields...> {
    template <class T>
    struct correct_field {
        using table_t = typename T::table_type;
        static constexpr bool is_field =
            std::is_base_of<table::detail::Field<typename T::type, table_t>, T>::value;
        static constexpr bool is_optional_field =
            std::is_base_of<table::detail::OptField<typename T::internal_type, table_t>, T>::value;

        static constexpr bool value = is_field || is_optional_field;
    };

    static_assert(utils::same_table<Fields...>::value, "all fields should be from the same table");
    static_assert(::utils::types::all_of<correct_field<Fields>...>::value,
                  "each field should be a subclass of either db::table::detail::Field<> "
                  "or OptField<>");

public:
    using return_type = std::tuple<Fields...>;

    std::string str() const {
        boost::format stmt("SELECT %1%\nFROM %2%\n%3%");
        stmt % utils::asSqlList({utils::getColumnName<Fields>()...});
        stmt % utils::getTableName<Fields...>();
        stmt % this->whereStmt();
        return stmt.str();
    }
};
}
}

#endif