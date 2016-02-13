#include "NewMapImagesResponse.h"

namespace server {
namespace io {
namespace output {

communication::NewMapImagesResponse NewMapImagesResponse::toThrift() const {
    communication::NewMapImagesResponse ret;
    for (const auto &e : floorImageUrls) {
        ret.floorImageUrls[e.first] = e.second;
    }
    ret.version = version;
    return ret;
}
}
}
}
