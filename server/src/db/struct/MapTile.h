#ifndef DB_STRUCT__MAP_TILE__H
#define DB_STRUCT__MAP_TILE__H

#include <cstdint>
#include <string>

namespace db {
struct MapTile {
    std::int32_t floor;
    std::int32_t zoomLevel;
    std::int32_t row;
    std::int32_t col;
    std::string filename;
};
}

#endif