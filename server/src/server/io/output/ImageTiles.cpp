#include "ImageTiles.h"

namespace server {
namespace io {
namespace output {

ImageTiles::ImageTiles(std::int32_t rows, std::int32_t columns) {
    tilesUrls.resize(rows);
    for (auto &row : tilesUrls) {
        row.resize(columns);
    }
}

communication::ImageTiles ImageTiles::toThrift() const {
    communication::ImageTiles res;
    res.scaledSize = scaledSize.toThrift();
    res.tileSize = tileSize.toThrift();
    res.tilesUrls = tilesUrls;
    return res;
}
}
}
}