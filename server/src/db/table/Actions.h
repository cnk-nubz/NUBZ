#ifndef DB_TABLE__ACTIONS__H
#define DB_TABLE__ACTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct Actions {
    struct FieldID : detail::Field<std::int32_t, Actions> {
        using detail::Field<std::int32_t, Actions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldID> ID{};

    struct FieldText : detail::Field<std::string, Actions> {
        using detail::Field<std::string, Actions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldText> Text{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldText>;
};
}
}

#endif