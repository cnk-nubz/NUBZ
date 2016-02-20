#ifndef DB_STRUCT__MAP_TILES_INFO__H
#define DB_STRUCT__MAP_TILES_INFO__H

#include <cstdint>

namespace db {
struct MapTilesInfo {
    std::int32_t floor;
    std::int32_t zoomLevel;
    std::int32_t rowsCount;
    std::int32_t colsCount;
    std::int32_t imgWidth;
    std::int32_t imgHeight;
    std::int32_t tileSize;
};
}

#endif