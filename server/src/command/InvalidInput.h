#ifndef COMMAND__INVALID_INPUT__H
#define COMMAND__INVALID_INPUT__H

#include <stdexcept>

namespace command {
    struct InvalidInput : public std::logic_error {
        explicit InvalidInput(const std::string &msg) : std::logic_error(msg) {
        }
        explicit InvalidInput(const char *msg) : std::logic_error(msg) {
        }
    };
}

#endif