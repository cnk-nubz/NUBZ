#ifndef IO__ACTION__H
#define IO__ACTION__H

#include <cstdint>

#include "communication/structs_types.h"
#include "db/struct/Action.h"

namespace io {
    struct Action {
        Action() = default;
        ~Action() = default;

        Action(const Action &) = default;
        Action(Action &&) = default;
        Action &operator=(const Action &) = default;
        Action &operator=(Action &&) = default;

        Action(const communication::Action &thrift);
        communication::Action toThrift() const;
        
        Action(const db::Action &dbAction);

        std::int32_t ID;
        std::string text;
    };
}

#endif