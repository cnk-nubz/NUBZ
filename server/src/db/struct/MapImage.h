#ifndef DB_STRUCT__MAP_IMAGE__H
#define DB_STRUCT__MAP_IMAGE__H

#include <cstdint>

namespace db {
    struct MapImage {
        std::string filename;
        std::uint32_t version;
        std::uint32_t level;
    };
}

#endif