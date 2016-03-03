#ifndef DB_TABLE__MULTIPLE_CHOICE_QUESTIONS__H
#define DB_TABLE__MULTIPLE_CHOICE_QUESTIONS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct MultipleChoiceQuestions {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueQuestion : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueSingleAnswer : detail::Value<bool> {
        using detail::Value<bool>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, MultipleChoiceQuestions, ValueId> {
        static const std::string name;
    };

    struct ColumnName : detail::Column<ColumnName, MultipleChoiceQuestions, ValueName> {
        static const std::string name;
    };

    struct ColumnQuestion : detail::Column<ColumnQuestion, MultipleChoiceQuestions, ValueQuestion> {
        static const std::string name;
    };

    struct ColumnSingleAnswer
        : detail::Column<ColumnSingleAnswer, MultipleChoiceQuestions, ValueSingleAnswer> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueName::type name;
        ValueQuestion::type question;
        ValueSingleAnswer::type singleAnswer;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueName, ValueQuestion, ValueSingleAnswer>;
        using DBIn = std::tuple<ValueName, ValueQuestion, ValueSingleAnswer>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnName colName;
    static const ColumnQuestion colQuestion;
    static const ColumnSingleAnswer colSingleAnswer;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnName, ColumnQuestion, ColumnSingleAnswer>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnName, ColumnQuestion, ColumnSingleAnswer>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnName, ColumnQuestion, ColumnSingleAnswer>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnName, ColumnQuestion, ColumnSingleAnswer>();
    }
};
}
}

#endif