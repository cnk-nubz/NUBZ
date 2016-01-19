#ifndef SERVER_IO__EXHIBITS_REQUEST__H
#define SERVER_IO__EXHIBITS_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class ExhibitsRequest {
public:
    ExhibitsRequest(const communication::ExhibitsRequest &thrift);

    boost::optional<std::int32_t> acquiredVersion;
};
}
}
}

#endif