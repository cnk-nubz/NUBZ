#ifndef REPOSITORY__MAP_IMAGES__H
#define REPOSITORY__MAP_IMAGES__H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

class MapImages {
public:
    struct MapImage {
        struct ZoomLevel {
            std::int32_t imageWidth;
            std::int32_t imageHeight;
            std::int32_t tileSize;

            std::vector<std::vector<std::string>> tilesFilenames;
        };

        std::int32_t floor;
        std::int32_t width;
        std::int32_t height;
        std::string filename;
        std::vector<ZoomLevel> zoomLevels;
    };

    MapImages(db::DatabaseSession &session);

    boost::optional<MapImage> get(std::int32_t floor);
    std::vector<MapImage> getAll();

    void remove(std::int32_t floor);
    void removeAll();

    void set(const MapImage &mapImage);

private:
    db::DatabaseSession &session;
};

using MapImage = MapImages::MapImage;
}

#endif