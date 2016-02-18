#ifndef SERVER_IO_INPUT__NEW_MAP_IMAGES_REQUEST__H
#define SERVER_IO_INPUT__NEW_MAP_IMAGES_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct NewMapImagesRequest {
    NewMapImagesRequest(const communication::NewMapImagesRequest &thrift);

    boost::optional<std::int32_t> acquiredVersion;
};
}
}
}

#endif