#ifndef DB_TABLE__SORT_QUESTION_OPTIONS__H
#define DB_TABLE__SORT_QUESTION_OPTIONS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

#include "SortQuestions.h"

namespace db {
namespace table {

struct SortQuestionOptions {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueQuestionId : detail::Value<SortQuestions::ValueId::internal_type> {
        using detail::Value<SortQuestions::ValueId::internal_type>::Value;
    };

    struct ValueText : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, SortQuestionOptions, ValueId> {
        static const std::string name;
    };

    struct ColumnQuestionId
        : detail::Column<ColumnQuestionId, SortQuestionOptions, ValueQuestionId> {
        static const std::string name;
    };

    struct ColumnText : detail::Column<ColumnText, SortQuestionOptions, ValueText> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueQuestionId::type questionID;
        ValueText::type text;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueQuestionId, ValueText>;
        using DBIn = std::tuple<ValueQuestionId, ValueText>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnQuestionId colQuestionId;
    static const ColumnText colText;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnQuestionId, ColumnText>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnQuestionId, ColumnText>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnQuestionId, ColumnText>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnQuestionId, ColumnText>();
    }
};
}
}

#endif