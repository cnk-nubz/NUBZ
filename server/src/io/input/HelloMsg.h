#ifndef IO__HELLO_MSG__H
#define IO__HELLO_MSG__H

#include <cstdint>
#include <iostream>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class HelloMsg {
        public:
            HelloMsg() = default;
            ~HelloMsg() = default;

            HelloMsg(const communication::HelloMsg &thrift);

            HelloMsg(const HelloMsg &) = default;
            HelloMsg(HelloMsg &&) = default;
            HelloMsg &operator=(const HelloMsg &) = default;
            HelloMsg &operator=(HelloMsg &&) = default;

            std::int32_t num;
            std::string msg;
        };
    }
}

#endif