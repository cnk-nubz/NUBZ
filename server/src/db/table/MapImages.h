#ifndef DB_TABLE__MAP_IMAGES__H
#define DB_TABLE__MAP_IMAGES__H

#include <cstdint>
#include <string>
#include <vector>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct MapImages {
    struct ZoomLevelsData {
        struct ZoomLevel {
            std::int32_t imageWidth;
            std::int32_t imageHeight;
            std::int32_t tileSize;
            std::vector<std::vector<std::string>> tilesFilenames;
        };

        ZoomLevelsData() = default;
        ZoomLevelsData(const std::string &jsonStr);
        operator std::string() const;

        std::vector<ZoomLevel> zoomLevels;
    };

    struct FieldFloor : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFloor> Floor{};

    struct FieldFilename : detail::Field<std::string, MapImages> {
        using detail::Field<std::string, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFilename> Filename{};

    struct FieldWidth : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldWidth> Width{};

    struct FieldHeight : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldHeight> Height{};

    struct FieldZoomLevels : detail::Field<ZoomLevelsData, MapImages> {
        using detail::Field<ZoomLevelsData, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldZoomLevels> ZoomLevels{};

    static const std::string tableName;

    using Sql =
        detail::SqlCore<FieldFloor, FieldFilename, FieldWidth, FieldHeight, FieldZoomLevels>;
};
}
}

#endif