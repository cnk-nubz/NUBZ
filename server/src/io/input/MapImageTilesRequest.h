#ifndef IO__MAP_IMAGE_TILES_REQUEST__H
#define IO__MAP_IMAGE_TILES_REQUEST__H

#include <cstdint>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class MapImageTilesRequest {
        public:
            MapImageTilesRequest() = default;
            ~MapImageTilesRequest() = default;

            MapImageTilesRequest(const communication::MapImageTilesRequest &thrift);

            MapImageTilesRequest(const MapImageTilesRequest &) = default;
            MapImageTilesRequest(MapImageTilesRequest &&) = default;
            MapImageTilesRequest &operator=(const MapImageTilesRequest &) = default;
            MapImageTilesRequest &operator=(MapImageTilesRequest &&) = default;

            std::int32_t floor;
        };
    }
}

#endif