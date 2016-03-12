#ifndef DB_TABLE__MULTIPLE_CHOICE_QUESTIONS__H
#define DB_TABLE__MULTIPLE_CHOICE_QUESTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct MultipleChoiceQuestions {
    struct FieldID : detail::Field<std::int32_t, MultipleChoiceQuestions> {
        using detail::Field<std::int32_t, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldID> ID{};

    struct FieldName : detail::Field<std::string, MultipleChoiceQuestions> {
        using detail::Field<std::string, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldName> Name{};

    struct FieldQuestion : detail::Field<std::string, MultipleChoiceQuestions> {
        using detail::Field<std::string, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldQuestion> Question{};

    struct FieldSingleAnswer : detail::Field<bool, MultipleChoiceQuestions> {
        using detail::Field<bool, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldSingleAnswer> SingleAnswer{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldName, FieldQuestion, FieldSingleAnswer>;
};
}
}

#endif