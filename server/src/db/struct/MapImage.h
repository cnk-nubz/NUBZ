#ifndef DB_STRUCT__MAP_IMAGE__H
#define DB_STRUCT__MAP_IMAGE__H

#include <cstdint>
#include <string>

namespace db {
struct MapImage {
    std::string filename;
    std::int32_t width;
    std::int32_t height;
    std::int32_t version;
    std::int32_t floor;
};
}

#endif