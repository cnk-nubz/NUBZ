#ifndef IO__MAP_IMAGE_TILES_RESPONSE__H
#define IO__MAP_IMAGE_TILES_RESPONSE__H

#include <vector>

#include "communication/structs_types.h"
#include "io/output/ImageTiles.h"
#include "io/Size.h"

namespace io {
    namespace output {
        struct MapImageTilesResponse {
            communication::MapImageTilesResponse toThrift() const;

            Size originalSize;
            std::vector<ImageTiles> zoomLevels;
        };
    }
}

#endif