#ifndef IO__INVALID_INPUT__H
#define IO__INVALID_INPUT__H

#include <stdexcept>

#include <communication/structs_types.h>

namespace server {
namespace io {

struct InvalidInput : public std::logic_error {
    using std::logic_error::logic_error;

    inline communication::InvalidData toThrift() const {
        return communication::InvalidData{};
    }
};
}
}

#endif