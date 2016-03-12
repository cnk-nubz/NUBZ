#ifndef DB_TABLE__MULTIPLE_CHOICE_QUESTION_OPTIONS__H
#define DB_TABLE__MULTIPLE_CHOICE_QUESTION_OPTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

/*
 * QuestionID references MultipleChoiceQuestions::ID (on delete cascade)
 */
struct MultipleChoiceQuestionOptions {
    struct FieldID : detail::Field<std::int32_t, MultipleChoiceQuestionOptions> {
        using detail::Field<std::int32_t, MultipleChoiceQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldID> ID{};

    struct FieldQuestionID : detail::Field<std::int32_t, MultipleChoiceQuestionOptions> {
        using detail::Field<std::int32_t, MultipleChoiceQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldQuestionID> QuestionID{};

    struct FieldText : detail::Field<std::string, MultipleChoiceQuestionOptions> {
        using detail::Field<std::string, MultipleChoiceQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldText> Text{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldQuestionID, FieldText>;
};
}
}

#endif