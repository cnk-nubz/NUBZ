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
    static constexpr detail::Column2<FieldFloor> Floor{};

    struct FieldFilename : detail::Field<std::string, MapImages> {
        using detail::Field<std::string, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldFilename> Filename{};

    struct FieldWidth : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldWidth> Width{};

    struct FieldHeight : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldHeight> Height{};

    struct FieldVersion : detail::Field<std::int32_t, MapImages> {
        using detail::Field<std::int32_t, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldVersion> Version{};

    struct FieldZoomLevels : detail::Field<ZoomLevelsData, MapImages> {
        using detail::Field<ZoomLevelsData, MapImages>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldZoomLevels> ZoomLevels{};

    static const std::string tableName;

    using Sql = detail::SqlCore<FieldFloor, FieldFilename, FieldWidth, FieldHeight, FieldVersion,
                                FieldZoomLevels>;
};
}
}

#endif