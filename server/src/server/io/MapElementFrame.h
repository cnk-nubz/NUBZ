#ifndef SERVER_IO__MAP_ELEMENT_FRAME__H
#define SERVER_IO__MAP_ELEMENT_FRAME__H

#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {

struct MapElementFrame {
    MapElementFrame() = default;

    MapElementFrame(const communication::MapElementFrame &thrift);
    communication::MapElementFrame toThrift() const;

    std::int32_t x;
    std::int32_t y;
    std::int32_t width;
    std::int32_t height;
    std::int32_t mapLevel;
};
}
}

#endif