#ifndef SERVER_IO__MAP_IMAGES_REQUEST__H
#define SERVER_IO__MAP_IMAGES_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class MapImagesRequest {
public:
    MapImagesRequest(const communication::MapImagesRequest &thrift);

    boost::optional<std::int32_t> acquiredVersion;
};
}
}
}

#endif