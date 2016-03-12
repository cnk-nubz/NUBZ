#ifndef DB_TABLE__SORT_QUESTIONS__H
#define DB_TABLE__SORT_QUESTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct SortQuestions {
    struct FieldID : detail::Field<std::int32_t, SortQuestions> {
        using detail::Field<std::int32_t, SortQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldID> ID{};

    struct FieldName : detail::Field<std::string, SortQuestions> {
        using detail::Field<std::string, SortQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldName> Name{};

    struct FieldQuestion : detail::Field<std::string, SortQuestions> {
        using detail::Field<std::string, SortQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldQuestion> Question{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldName, FieldQuestion>;
};
}
}

#endif