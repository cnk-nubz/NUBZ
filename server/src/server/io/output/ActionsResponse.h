#ifndef IO__ACTIONS_RESPONSE__H
#define IO__ACTIONS_RESPONSE__H

#include <vector>

#include <communication/structs_types.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

class ActionsResponse {
public:
    communication::ActionsResponse toThrift() const;

    std::vector<io::Action> actions;
};
}
}
}

#endif