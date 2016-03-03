#ifndef DB_TABLE__COUNTERS__H
#define DB_TABLE__COUNTERS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct Counters {
    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueCounter : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ColumnName : detail::Column<ColumnName, Counters, ValueName> {
        static const std::string name;
    };

    struct ColumnCounter : detail::Column<ColumnCounter, Counters, ValueCounter> {
        static const std::string name;
    };

    struct Row {
        ValueName::type name;
        ValueCounter::type counter;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueName, ValueCounter>;
        using DBIn = std::tuple<ValueName, ValueCounter>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnName colName;
    static const ColumnCounter colCounter;

    static auto select() {
        return ::db::sql::Select<ColumnName, ColumnCounter>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnName, ColumnCounter>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnName, ColumnCounter>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnName, ColumnCounter>();
    }
};
}
}

#endif