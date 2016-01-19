#ifndef SERVER_IO__EXHIBIT__H
#define SERVER_IO__EXHIBIT__H

#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include "MapElementFrame.h"

namespace server {
namespace io {

struct Exhibit {
    Exhibit() = default;

    Exhibit(const communication::Exhibit &thrift);
    communication::Exhibit toThrift() const;

    std::string name;
    boost::optional<MapElementFrame> frame;
};
}
}

#endif