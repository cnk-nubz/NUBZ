#ifndef IO__SET_MAP_IMAGE_REQUEST__H
#define IO__SET_MAP_IMAGE_REQUEST__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class SetMapImageRequest {
        public:
            SetMapImageRequest() = default;
            ~SetMapImageRequest() = default;

            SetMapImageRequest(const communication::SetMapImageRequest &thrift);

            SetMapImageRequest(const SetMapImageRequest &) = default;
            SetMapImageRequest(SetMapImageRequest &&) = default;
            SetMapImageRequest &operator=(const SetMapImageRequest &) = default;
            SetMapImageRequest &operator=(SetMapImageRequest &&) = default;

            std::int32_t level;
            std::string filename;
        };
    }
}

#endif