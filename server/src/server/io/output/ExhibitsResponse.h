#ifndef IO__EXHIBITS_RESPONSE__H
#define IO__EXHIBITS_RESPONSE__H

#include <cstdint>
#include <map>

#include <boost/optional.hpp>

#include <communication/structs_types.h>
#include <server/io/Exhibit.h>

namespace server {
namespace io {
namespace output {

struct ExhibitsResponse {
    communication::ExhibitsResponse toThrift() const;

    std::int32_t version;
    std::map<std::int32_t, Exhibit> exhibits;
};
}
}
}

#endif