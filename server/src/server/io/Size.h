#ifndef SERVER_IO__SIZE__H
#define SERVER_IO__SIZE__H

#include <cstdint>

#include <communication/structs_types.h>

namespace server {
namespace io {

struct Size {
    using thrift_t = communication::Size;

    Size() = default;
    Size(const thrift_t &thrift);
    thrift_t toThrift() const;

    std::int32_t width;
    std::int32_t height;
};
}
}

#endif