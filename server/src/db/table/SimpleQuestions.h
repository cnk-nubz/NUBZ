#ifndef DB_TABLE__SIMPLE_QUESTIONS__H
#define DB_TABLE__SIMPLE_QUESTIONS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct SimpleQuestions {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueQuestion : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueNumberAnswer : detail::Value<bool> {
        using detail::Value<bool>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, SimpleQuestions, ValueId> {
        static const std::string name;
    };

    struct ColumnName : detail::Column<ColumnName, SimpleQuestions, ValueName> {
        static const std::string name;
    };

    struct ColumnQuestion : detail::Column<ColumnQuestion, SimpleQuestions, ValueQuestion> {
        static const std::string name;
    };

    struct ColumnNumberAnswer
        : detail::Column<ColumnNumberAnswer, SimpleQuestions, ValueNumberAnswer> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueName::type name;
        ValueQuestion::type question;
        ValueNumberAnswer::type numberAnswer;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueName, ValueQuestion, ValueNumberAnswer>;
        using DBIn = std::tuple<ValueName, ValueQuestion, ValueNumberAnswer>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnName colName;
    static const ColumnQuestion colQuestion;
    static const ColumnNumberAnswer colNumberAnswer;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnName, ColumnQuestion, ColumnNumberAnswer>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnName, ColumnQuestion, ColumnNumberAnswer>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnName, ColumnQuestion, ColumnNumberAnswer>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnName, ColumnQuestion, ColumnNumberAnswer>();
    }
};
}
}

#endif