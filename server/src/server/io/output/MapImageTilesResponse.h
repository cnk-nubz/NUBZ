#ifndef SERVER_IO_OUTPUT__MAP_IMAGE_TILES_RESPONSE__H
#define SERVER_IO_OUTPUT__MAP_IMAGE_TILES_RESPONSE__H

#include <cstdint>
#include <string>
#include <vector>

#include <communication/structs_types.h>

#include <server/io/Size.h>

namespace server {
namespace io {
namespace output {

struct MapImageTilesResponse {
    struct ImageTiles {
        using thrift_t = communication::ImageTiles;

        ImageTiles(std::int32_t rows, std::int32_t columns);
        thrift_t toThrift() const;

        Size scaledSize;
        Size tileSize;
        std::vector<std::vector<std::string>> tilesUrls;
    };

    communication::MapImageTilesResponse toThrift() const;

    Size originalSize;
    std::vector<ImageTiles> zoomLevels;
};
}
}
}

#endif