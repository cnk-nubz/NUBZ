#ifndef SERVER_IO_INPUT__CREATE_ACTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_ACTION_REQUEST__H

#include <string>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct CreateActionRequest {
    CreateActionRequest(const communication::CreateActionRequest &thrift);

    std::string text;
};
}
}
}

#endif