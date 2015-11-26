#ifndef IO__MAP_IMAGES_REQUEST__H
#define IO__MAP_IMAGES_REQUEST__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class MapImagesRequest {
        public:
            MapImagesRequest() = default;
            ~MapImagesRequest() = default;

            MapImagesRequest(const communication::MapImagesRequest &thrift);

            MapImagesRequest(const MapImagesRequest &) = default;
            MapImagesRequest(MapImagesRequest &&) = default;
            MapImagesRequest &operator=(const MapImagesRequest &) = default;
            MapImagesRequest &operator=(MapImagesRequest &&) = default;

            boost::optional<std::int32_t> acquiredVersion;
        };
    }
}

#endif