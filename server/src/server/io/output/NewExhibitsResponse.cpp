#include "NewExhibitsResponse.h"

namespace server {
namespace io {
namespace output {

communication::NewExhibitsResponse NewExhibitsResponse::toThrift() const {
    auto res = communication::NewExhibitsResponse{};
    res.version = version;
    for (auto &ex : exhibits) {
        res.exhibits[ex.first] = ex.second.toThrift();
    }
    res.fullRefresh = fullRefresh;
    return res;
}
}
}
}