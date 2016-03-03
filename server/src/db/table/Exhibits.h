#ifndef DB_TABLE__EXHIBITS__H
#define DB_TABLE__EXHIBITS__H

#include <tuple>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct Exhibits {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueVersion : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueFrameX : detail::OptValue<std::int32_t> {
        using detail::OptValue<std::int32_t>::OptValue;
    };

    struct ValueFrameY : detail::OptValue<std::int32_t> {
        using detail::OptValue<std::int32_t>::OptValue;
    };

    struct ValueFrameWidth : detail::OptValue<std::int32_t> {
        using detail::OptValue<std::int32_t>::OptValue;
    };

    struct ValueFrameHeight : detail::OptValue<std::int32_t> {
        using detail::OptValue<std::int32_t>::OptValue;
    };

    struct ValueFrameFloor : detail::OptValue<std::int32_t> {
        using detail::OptValue<std::int32_t>::OptValue;
    };

    struct ColumnId : detail::Column<ColumnId, Exhibits, ValueId> {
        static const std::string name;
    };

    struct ColumnName : detail::Column<ColumnName, Exhibits, ValueName> {
        static const std::string name;
    };

    struct ColumnVersion : detail::Column<ColumnVersion, Exhibits, ValueVersion> {
        static const std::string name;
    };

    struct ColumnFrameX : detail::Column<ColumnFrameX, Exhibits, ValueFrameX> {
        static const std::string name;
    };

    struct ColumnFrameY : detail::Column<ColumnFrameY, Exhibits, ValueFrameY> {
        static const std::string name;
    };

    struct ColumnFrameWidth : detail::Column<ColumnFrameWidth, Exhibits, ValueFrameWidth> {
        static const std::string name;
    };

    struct ColumnFrameHeight : detail::Column<ColumnFrameHeight, Exhibits, ValueFrameHeight> {
        static const std::string name;
    };

    struct ColumnFrameFloor : detail::Column<ColumnFrameFloor, Exhibits, ValueFrameFloor> {
        static const std::string name;
    };

    struct Row {
        ValueId::type ID;
        ValueName::type name;
        ValueVersion::type version;

        ValueFrameX::type frameX;
        ValueFrameY::type frameY;
        ValueFrameWidth::type frameWidth;
        ValueFrameHeight::type frameHeight;
        ValueFrameFloor::type frameFloor;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueName, ValueVersion, ValueFrameX, ValueFrameY,
                                 ValueFrameWidth, ValueFrameHeight, ValueFrameFloor>;
        using DBIn = std::tuple<ValueName, ValueVersion, ValueFrameX, ValueFrameY, ValueFrameWidth,
                                ValueFrameHeight, ValueFrameFloor>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnName colName;
    static const ColumnVersion colVersion;

    static const ColumnFrameX colFrameX;
    static const ColumnFrameY colFrameY;
    static const ColumnFrameWidth colFrameWidth;
    static const ColumnFrameHeight colFrameHeight;
    static const ColumnFrameFloor colFrameFloor;

    static auto select() {
        return ::db::sql::Select<ColumnId,
                                 ColumnName,
                                 ColumnVersion,
                                 ColumnFrameX,
                                 ColumnFrameY,
                                 ColumnFrameWidth,
                                 ColumnFrameHeight,
                                 ColumnFrameFloor>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnName,
                                 ColumnVersion,
                                 ColumnFrameX,
                                 ColumnFrameY,
                                 ColumnFrameWidth,
                                 ColumnFrameHeight,
                                 ColumnFrameFloor>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId,
                                 ColumnName,
                                 ColumnVersion,
                                 ColumnFrameX,
                                 ColumnFrameY,
                                 ColumnFrameWidth,
                                 ColumnFrameHeight,
                                 ColumnFrameFloor>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId,
                                 ColumnName,
                                 ColumnVersion,
                                 ColumnFrameX,
                                 ColumnFrameY,
                                 ColumnFrameWidth,
                                 ColumnFrameHeight,
                                 ColumnFrameFloor>();
    }
};
}
}

#endif