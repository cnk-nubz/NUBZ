#include "MapImageTilesResponse.h"

namespace io {
    namespace output {
        communication::MapImageTilesResponse MapImageTilesResponse::toThrift() const {
            communication::MapImageTilesResponse res;
            res.originalSize = originalSize.toThrift();
            for (const auto &zoomLevel : zoomLevels) {
                res.zoomLevels.push_back(zoomLevel.toThrift());
            }
            return res;
        }
    }
}