#ifndef IO__INVALID_INPUT__H
#define IO__INVALID_INPUT__H

#include <stdexcept>

#include "communication/structs_types.h"

namespace io {
    struct InvalidInput : public std::logic_error {
        inline explicit InvalidInput(const std::string &msg) : std::logic_error(msg) {
        }

        inline explicit InvalidInput(const char *msg) : std::logic_error(msg) {
        }

        inline communication::InvalidData toThrift() const {
            return communication::InvalidData{};
        }
    };
}

#endif