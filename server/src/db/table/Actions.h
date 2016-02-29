#ifndef DB_TABLE__ACTIONS__H
#define DB_TABLE__ACTIONS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct Actions {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueText : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, Actions, ValueId> {
        static const std::string name;
    };

    struct ColumnText : detail::Column<ColumnText, Actions, ValueText> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueText::type text;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueText>;
        using DBIn = std::tuple<ValueText>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnText colText;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnText>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnText>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnText>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnText>();
    }
};
}
}

#endif