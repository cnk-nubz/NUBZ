#ifndef SERVER_IO__EXHIBIT__H
#define SERVER_IO__EXHIBIT__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <repository/Exhibits.h>

#include <communication/structs_types.h>

#include "MapFrame.h"

namespace server {
namespace io {

struct Exhibit {
    using thrift_t = communication::Exhibit;
    using repo_t = repository::Exhibit;

    Exhibit() = default;
    thrift_t toThrift() const;

    Exhibit(const repo_t &repo);

    std::int32_t ID;
    std::string name;
    boost::optional<MapFrame> mapFrame;
};
}
}

#endif