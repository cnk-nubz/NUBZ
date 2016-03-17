#ifndef SERVER_IO_INPUT__HELLO_MSG__H
#define SERVER_IO_INPUT__HELLO_MSG__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct HelloMsg {
    HelloMsg(const communication::HelloMsg &thrift);

    std::int32_t num;
    std::string msg;
};
}
}
}

#endif