#ifndef SERVER_IO__ACTION__H
#define SERVER_IO__ACTION__H

#include <cstdint>

#include <communication/structs_types.h>
#include <db/struct/Action.h>

namespace server {
namespace io {

struct Action {
    Action() = default;

    Action(const communication::Action &thrift);
    communication::Action toThrift() const;

    std::int32_t ID;
    std::string text;
};
}
}

#endif