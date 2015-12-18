#ifndef IO__SIZE__H
#define IO__SIZE__H

#include <cstdint>

#include "communication/structs_types.h"

namespace io {
    struct Size {
        Size() = default;
        ~Size() = default;

        Size(const Size &) = default;
        Size(Size &&) = default;
        Size &operator=(const Size &) = default;
        Size &operator=(Size &&) = default;

        Size(const communication::Size &thrift);
        communication::Size toThrift() const;

        std::int32_t width;
        std::int32_t height;
    };
}

#endif