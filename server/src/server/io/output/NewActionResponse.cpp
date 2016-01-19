#include "NewActionResponse.h"

namespace server {
namespace io {
namespace output {

communication::NewActionResponse NewActionResponse::toThrift() const {
    communication::NewActionResponse res;
    res.action = action.toThrift();
    return res;
}
}
}
}