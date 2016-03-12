#ifndef DB_TABLE__COUNTERS__H
#define DB_TABLE__COUNTERS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct Counters {
    struct FieldName : detail::Field<std::string, Counters> {
        using detail::Field<std::string, Counters>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldName> Name{};

    struct FieldCounter : detail::Field<std::int32_t, Counters> {
        using detail::Field<std::int32_t, Counters>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldCounter> Counter{};

    static const std::string tableName;

    using Sql = detail::SqlCore<FieldName, FieldCounter>;
};
}
}

#endif