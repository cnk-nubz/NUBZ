#ifndef SERVER_IO_INPUT__CLONE_REQUEST__H
#define SERVER_IO_INPUT__CLONE_REQUEST__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct CloneRequest {
    CloneRequest(const communication::CloneRequest &thrift);

    const std::int32_t ID;
    const std::string name;
};
}
}
}

#endif