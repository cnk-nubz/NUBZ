#ifndef SERVER_IO_INPUT__NEW_EXHIBITS_REQUEST__H
#define SERVER_IO_INPUT__NEW_EXHIBITS_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct NewExhibitsRequest {
    NewExhibitsRequest(const communication::NewExhibitsRequest &thrift);

    boost::optional<std::int32_t> acquiredVersion;
};
}
}
}

#endif