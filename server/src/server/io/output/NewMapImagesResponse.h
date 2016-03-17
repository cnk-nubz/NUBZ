#ifndef SERVER_IO_OUTPUT__NEW_MAP_IMAGES_RESPONSE__H
#define SERVER_IO_OUTPUT__NEW_MAP_IMAGES_RESPONSE__H

#include <cstdint>
#include <string>
#include <unordered_map>

#include <communication/structs_types.h>

#include "MapImage.h"

namespace server {
namespace io {
namespace output {

struct NewMapImagesResponse {
    communication::NewMapImagesResponse toThrift() const;

    std::int32_t version;
    std::unordered_map<std::int32_t, MapImage> floors;
};
}
}
}

#endif