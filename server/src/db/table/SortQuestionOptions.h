#ifndef DB_TABLE__SORT_QUESTION_OPTIONS__H
#define DB_TABLE__SORT_QUESTION_OPTIONS__H

#include <cstdint>
#include <string>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

/*
 * QuestionID references SortQuestions::ID (on delete cascade)
 */
struct SortQuestionOptions {
    struct FieldID : detail::Field<std::int32_t, SortQuestionOptions> {
        using detail::Field<std::int32_t, SortQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldID> ID{};

    struct FieldQuestionID : detail::Field<std::int32_t, SortQuestionOptions> {
        using detail::Field<std::int32_t, SortQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldQuestionID> QuestionID{};

    struct FieldText : detail::Field<std::string, SortQuestionOptions> {
        using detail::Field<std::string, SortQuestionOptions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldText> Text{};

    static const std::string tableName;

    using Sql = detail::SqlCoreID<FieldID, FieldQuestionID, FieldText>;
};
}
}

#endif