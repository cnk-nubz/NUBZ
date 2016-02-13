#ifndef SERVER_IO_INPUT__SET_MAP_IMAGE_REQUEST__H
#define SERVER_IO_INPUT__SET_MAP_IMAGE_REQUEST__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct SetMapImageRequest {
    SetMapImageRequest(const communication::SetMapImageRequest &thrift);

    std::int32_t floor;
    std::string filename;
};
}
}
}

#endif