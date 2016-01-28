#include "NewExhibitsResponse.h"

namespace server {
namespace io {
namespace output {

communication::NewExhibitsResponse NewExhibitsResponse::toThrift() const {
    communication::NewExhibitsResponse res;
    res.version = version;
    for (auto &ex : exhibits) {
        res.exhibits[ex.first] = ex.second.toThrift();
    }
    return res;
}
}
}
}