#ifndef SERVER_IO_OUTPUT__NEW_EXHIBITS_RESPONSE__H
#define SERVER_IO_OUTPUT__NEW_EXHIBITS_RESPONSE__H

#include <cstdint>
#include <unordered_map>

#include <communication/structs_types.h>

#include <server/io/Exhibit.h>

namespace server {
namespace io {
namespace output {

struct NewExhibitsResponse {
    communication::NewExhibitsResponse toThrift() const;

    std::int32_t version;
    std::unordered_map<std::int32_t, Exhibit> exhibits;
};
}
}
}

#endif