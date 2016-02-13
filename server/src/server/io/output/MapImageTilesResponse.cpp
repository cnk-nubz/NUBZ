#include <server/io/utils.h>

#include "MapImageTilesResponse.h"

namespace server {
namespace io {
namespace output {

communication::MapImageTilesResponse MapImageTilesResponse::toThrift() const {
    communication::MapImageTilesResponse res;
    res.originalSize = originalSize.toThrift();
    res.zoomLevels = ioToThrift(zoomLevels);
    return res;
}

MapImageTilesResponse::ImageTiles::ImageTiles(std::int32_t rows, std::int32_t columns) {
    tilesUrls.resize(rows);
    for (auto &row : tilesUrls) {
        row.resize(columns);
    }
}

communication::ImageTiles MapImageTilesResponse::ImageTiles::toThrift() const {
    communication::ImageTiles res;
    res.scaledSize = scaledSize.toThrift();
    res.tileSize = tileSize.toThrift();
    res.tilesUrls = tilesUrls;
    return res;
}
}
}
}