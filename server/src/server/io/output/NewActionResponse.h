#ifndef IO__NEW_ACTION_RESPONSE__H
#define IO__NEW_ACTION_RESPONSE__H

#include <communication/structs_types.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

class NewActionResponse {
public:
    communication::NewActionResponse toThrift() const;

    io::Action action;
};
}
}
}

#endif