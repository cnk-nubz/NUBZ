#ifndef SERVER_IO__QUESTION__H
#define SERVER_IO__QUESTION__H

#include <cstdint>
#include <string>

#include <server/utils/CmpUTF8.h>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace output {

struct Question {
    std::int32_t ID;
    std::string name;
    std::string question;

    bool operator<(const Question &that) const {
        return utils::cmpUTF8(name, that.name);
    }
};
}
}
}

#endif