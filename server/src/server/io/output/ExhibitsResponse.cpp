#include "ExhibitsResponse.h"

namespace server {
namespace io {
namespace output {

communication::ExhibitsResponse ExhibitsResponse::toThrift() const {
    communication::ExhibitsResponse ret;
    ret.version = version;
    for (auto &ex : exhibits) {
        ret.exhibits[ex.first] = ex.second.toThrift();
    }
    return ret;
}
}
}
}