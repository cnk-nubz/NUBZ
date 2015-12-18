#ifndef IO__IMAGE_TILES__H
#define IO__IMAGE_TILES__H

#include <vector>
#include <string>
#include <cstdint>

#include "communication/structs_types.h"
#include "io/Size.h"

namespace io {
    namespace output {
        struct ImageTiles {
            ImageTiles(std::int32_t rows, std::int32_t columns);
            ~ImageTiles() = default;

            ImageTiles(const ImageTiles &) = default;
            ImageTiles(ImageTiles &&) = default;
            ImageTiles &operator=(const ImageTiles &) = default;
            ImageTiles &operator=(ImageTiles &&) = default;

            communication::ImageTiles toThrift() const;

            Size scaledSize;
            Size tileSize;
            std::vector<std::vector<std::string>> tilesUrls;
        };
    }
}

#endif