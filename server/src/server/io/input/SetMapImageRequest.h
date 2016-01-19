#ifndef SERVER_IO__SET_MAP_IMAGE_REQUEST__H
#define SERVER_IO__SET_MAP_IMAGE_REQUEST__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class SetMapImageRequest {
public:
    SetMapImageRequest(const communication::SetMapImageRequest &thrift);

    std::int32_t level;
    std::string filename;
};
}
}
}

#endif