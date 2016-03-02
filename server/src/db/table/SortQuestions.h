#ifndef DB_TABLE__SORT_QUESTIONS__H
#define DB_TABLE__SORT_QUESTIONS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct SortQuestions {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueQuestion : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, SortQuestions, ValueId> {
        static const std::string name;
    };

    struct ColumnName : detail::Column<ColumnName, SortQuestions, ValueName> {
        static const std::string name;
    };

    struct ColumnQuestion : detail::Column<ColumnQuestion, SortQuestions, ValueQuestion> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueName::type name;
        ValueQuestion::type question;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueName, ValueQuestion>;
        using DBIn = std::tuple<ValueName, ValueQuestion>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnName colName;
    static const ColumnQuestion colQuestion;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnName, ColumnQuestion>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnName, ColumnQuestion>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnName, ColumnQuestion>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnName, ColumnQuestion>();
    }
};
}
}

#endif