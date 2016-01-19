#ifndef SERVER_IO__SIZE__H
#define SERVER_IO__SIZE__H

#include <cstdint>

#include <communication/structs_types.h>

namespace server {
namespace io {

struct Size {
    Size() = default;

    Size(const communication::Size &thrift);
    communication::Size toThrift() const;

    std::int32_t width;
    std::int32_t height;
};
}
}

#endif