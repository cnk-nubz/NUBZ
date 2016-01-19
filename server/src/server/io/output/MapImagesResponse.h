#ifndef IO__MAP_IMAGES_RESPONSE__H
#define IO__MAP_IMAGES_RESPONSE__H

#include <cstdint>
#include <map>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace output {

struct MapImagesResponse {
    communication::MapImagesResponse toThrift() const;

    std::int32_t version;
    std::map<std::int32_t, std::string> levelImageUrls;
};
}
}
}

#endif