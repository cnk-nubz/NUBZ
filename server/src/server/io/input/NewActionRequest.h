#ifndef SERVER_IO__NEW_ACTION_REQUEST__H
#define SERVER_IO__NEW_ACTION_REQUEST__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class NewActionRequest {
public:
    NewActionRequest(const communication::NewActionRequest &thrift);

    std::string text;
};
}
}
}

#endif