#ifndef SERVER_IO__MAP_FRAME__H
#define SERVER_IO__MAP_FRAME__H

#include <cstdint>

#include <communication/structs_types.h>

#include "Frame.h"

namespace server {
namespace io {

struct MapFrame {
    using thrift_t = communication::MapFrame;

    MapFrame() = default;
    MapFrame(const thrift_t &thrift);
    thrift_t toThrift() const;

    Frame frame;
    std::int32_t floor;
};
}
}

#endif