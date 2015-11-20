#ifndef IO__MAP_IMAGES_RESPONSE__H
#define IO__MAP_IMAGES_RESPONSE__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include "communication/structs_types.h"

namespace io {
    namespace output {
        class MapImagesResponse {
        public:
            MapImagesResponse() = default;
            ~MapImagesResponse() = default;

            MapImagesResponse(const MapImagesResponse &) = default;
            MapImagesResponse(MapImagesResponse &&) = default;
            MapImagesResponse &operator=(const MapImagesResponse &) = default;
            MapImagesResponse &operator=(MapImagesResponse &&) = default;

            communication::MapImagesResponse toThrift() const;

            std::int32_t version;
            std::map<std::int32_t, std::string> levelImageUrls;
        };
    }
}

#endif