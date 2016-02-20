#ifndef DB_STRUCT__MAP_ELEMENT_FRAME__H
#define DB_STRUCT__MAP_ELEMENT_FRAME__H

#include <cstdint>

namespace db {
struct MapFrame {
    std::int32_t x;
    std::int32_t y;
    std::int32_t width;
    std::int32_t height;
    std::int32_t floor;
};
}

#endif