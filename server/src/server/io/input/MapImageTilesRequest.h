#ifndef SERVER_IO__MAP_IMAGE_TILES_REQUEST__H
#define SERVER_IO__MAP_IMAGE_TILES_REQUEST__H

#include <cstdint>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class MapImageTilesRequest {
public:
    MapImageTilesRequest(const communication::MapImageTilesRequest &thrift);

    std::int32_t floor;
};
}
}
}

#endif