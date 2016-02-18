#ifndef SERVER_IO__FRAME__H
#define SERVER_IO__FRAME__H

#include <cstdint>

#include <communication/structs_types.h>

#include "Size.h"

namespace server {
namespace io {

struct Frame {
    using thrift_t = communication::Frame;

    Frame() = default;
    Frame(const thrift_t &thrift);
    thrift_t toThrift() const;

    std::int32_t x;
    std::int32_t y;
    Size size;
};
}
}

#endif