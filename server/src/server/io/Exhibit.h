#ifndef SERVER_IO__EXHIBIT__H
#define SERVER_IO__EXHIBIT__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include "MapFrame.h"

namespace server {
namespace io {

struct Exhibit {
    using thrift_t = communication::Exhibit;

    Exhibit() = default;
    Exhibit(const thrift_t &thrift);
    thrift_t toThrift() const;

    std::int32_t ID;
    std::string name;
    boost::optional<MapFrame> mapFrame;
};
}
}

#endif