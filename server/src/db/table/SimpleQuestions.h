#ifndef DB_TABLE__SIMPLE_QUESTIONS__H
#define DB_TABLE__SIMPLE_QUESTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct SimpleQuestions {
    struct FieldID : detail::Field<std::int32_t, SimpleQuestions> {
        using detail::Field<std::int32_t, SimpleQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldID> ID{};

    struct FieldName : detail::Field<std::string, SimpleQuestions> {
        using detail::Field<std::string, SimpleQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldName> Name{};

    struct FieldQuestion : detail::Field<std::string, SimpleQuestions> {
        using detail::Field<std::string, SimpleQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldQuestion> Question{};

    struct FieldNumberAnswer : detail::Field<bool, SimpleQuestions> {
        using detail::Field<bool, SimpleQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldNumberAnswer> NumberAnswer{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldName, FieldQuestion, FieldNumberAnswer>;
};
}
}

#endif