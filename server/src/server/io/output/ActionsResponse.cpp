#include "ActionsResponse.h"

namespace server {
namespace io {
namespace output {

communication::ActionsResponse ActionsResponse::toThrift() const {
    communication::ActionsResponse res;
    for (const auto &action : actions) {
        res.actions.emplace_back(action.toThrift());
    }
    return res;
}
}
}
}