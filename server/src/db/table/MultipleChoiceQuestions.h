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
    static constexpr detail::Column<FieldID> ID{};

    struct FieldName : detail::Field<std::string, MultipleChoiceQuestions> {
        using detail::Field<std::string, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldName> Name{};

    struct FieldQuestion : detail::Field<std::string, MultipleChoiceQuestions> {
        using detail::Field<std::string, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldQuestion> Question{};

    struct FieldSingleAnswer : detail::Field<bool, MultipleChoiceQuestions> {
        using detail::Field<bool, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldSingleAnswer> SingleAnswer{};

    // default = 0
    struct FieldRefCount : detail::Field<std::int32_t, MultipleChoiceQuestions> {
        using detail::Field<std::int32_t, MultipleChoiceQuestions>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldRefCount> RefCount{};

    static const std::string tableName;

    using Sql = detail::SqlCoreIDRefCount<FieldID, FieldRefCount, FieldName, FieldQuestion,
                                          FieldSingleAnswer>;
};
}
}

#endif