#ifndef SERVER_IO_OUTPUT__MAP_IMAGE__H
#define SERVER_IO_OUTPUT__MAP_IMAGE__H

#include <cstdint>
#include <string>
#include <vector>

#include <communication/structs_types.h>

#include <repository/MapImages.h>

#include <server/io/Size.h>

namespace server {
namespace io {
namespace output {

struct MapImage {
    struct ZoomLevel {
        using thrift_t = communication::ZoomLevel;
        using repo_t = repository::MapImage::ZoomLevel;

        ZoomLevel() = default;
        thrift_t toThrift() const;

        // translates filenames to urls
        ZoomLevel(const repo_t &repo);

        Size scaledResolution;
        Size tileSize;
        std::vector<std::vector<std::string>> tilesUrls;
    };

    using thrift_t = communication::MapImage;
    using repo_t = repository::MapImage;

    MapImage() = default;
    thrift_t toThrift() const;

    MapImage(const repo_t &repo);

    std::int32_t floor;
    Size resolution;
    std::vector<ZoomLevel> zoomLevels;
};
}
}
}

#endif