#ifndef MAP_IMAGE__H
#define MAP_IMAGE__H

#include <cstdint>

struct MapImage {
    std::string filename;
    std::uint32_t version;
    std::uint32_t level;
};

#endif