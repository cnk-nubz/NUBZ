#include "NewMapImagesResponse.h"

namespace server {
namespace io {
namespace output {

communication::NewMapImagesResponse NewMapImagesResponse::toThrift() const {
    auto res = communication::NewMapImagesResponse{};
    for (const auto &e : floors) {
        res.floors[e.first] = e.second.toThrift();
    }
    res.version = version;
    return res;
}
}
}
}
