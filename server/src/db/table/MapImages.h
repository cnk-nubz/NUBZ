#ifndef DB_TABLE__MAP_IMAGES__H
#define DB_TABLE__MAP_IMAGES__H

#include <tuple>
#include <vector>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct MapImages {
    struct ValueFloor : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueFilename : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueWidth : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueHeight : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueVersion : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueZoomLevels : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnFloor : detail::Column<ColumnFloor, MapImages, ValueFloor> {
        static const std::string name;
    };

    struct ColumnFilename : detail::Column<ColumnFilename, MapImages, ValueFilename> {
        static const std::string name;
    };

    struct ColumnWidth : detail::Column<ColumnWidth, MapImages, ValueWidth> {
        static const std::string name;
    };

    struct ColumnHeight : detail::Column<ColumnHeight, MapImages, ValueHeight> {
        static const std::string name;
    };

    struct ColumnVersion : detail::Column<ColumnVersion, MapImages, ValueVersion> {
        static const std::string name;
    };

    struct ColumnZoomLevels : detail::Column<ColumnZoomLevels, MapImages, ValueZoomLevels> {
        static const std::string name;
    };

    struct Row {
        struct ZoomLevel {
            std::int32_t imageWidth;
            std::int32_t imageHeight;
            std::int32_t tileSize;
            std::vector<std::vector<std::string>> tilesFilenames;
        };

        ValueFloor::type floor;
        ValueFilename::type filename;
        ValueWidth::type width;
        ValueHeight::type height;
        ValueVersion::type version;
        std::vector<ZoomLevel> zoomLevels;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueFloor, ValueFilename, ValueWidth, ValueHeight, ValueVersion,
                                 ValueZoomLevels>;
        using DBIn = std::tuple<ValueFloor, ValueFilename, ValueWidth, ValueHeight, ValueVersion,
                                ValueZoomLevels>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnFloor colFloor;
    static const ColumnFilename colFilename;
    static const ColumnWidth colWidth;
    static const ColumnHeight colHeight;
    static const ColumnVersion colVersion;
    static const ColumnZoomLevels colZoomLevels;

    static auto select() {
        return ::db::sql::Select<ColumnFloor,
                                 ColumnFilename,
                                 ColumnWidth,
                                 ColumnHeight,
                                 ColumnVersion,
                                 ColumnZoomLevels>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnFloor,
                                 ColumnFilename,
                                 ColumnWidth,
                                 ColumnHeight,
                                 ColumnVersion,
                                 ColumnZoomLevels>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnFloor,
                                 ColumnFilename,
                                 ColumnWidth,
                                 ColumnHeight,
                                 ColumnVersion,
                                 ColumnZoomLevels>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnFloor,
                                 ColumnFilename,
                                 ColumnWidth,
                                 ColumnHeight,
                                 ColumnVersion,
                                 ColumnZoomLevels>();
    }
};
}
}

#endif