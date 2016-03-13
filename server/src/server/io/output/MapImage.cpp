#include <server/io/utils.h>
#include <server/utils/PathHelper.h>

#include "MapImage.h"

namespace server {
namespace io {
namespace output {

MapImage::MapImage(const repository::MapImage &repo) : floor(repo.floor) {
    resolution.width = repo.width;
    resolution.height = repo.height;
    zoomLevels = std::vector<MapImage::ZoomLevel>(repo.zoomLevels.begin(), repo.zoomLevels.end());
}

communication::MapImage MapImage::toThrift() const {
    auto res = communication::MapImage{};
    res.floor = floor;
    res.resolution = resolution.toThrift();
    res.zoomLevels = ioToThrift(zoomLevels);
    return res;
}

MapImage::ZoomLevel::ZoomLevel(const repository::MapImage::ZoomLevel &repo) {
    scaledResolution.width = repo.imageWidth;
    scaledResolution.height = repo.imageHeight;
    tileSize.width = tileSize.height = repo.tileSize;
    tilesUrls = repo.tilesFilenames;
    for (auto &row : tilesUrls) {
        for (auto &entry : row) {
            auto url = server::utils::PathHelper::tilesUrl.urlFor(entry);
            entry = url;
        }
    }
}

communication::ZoomLevel MapImage::ZoomLevel::toThrift() const {
    auto res = communication::ZoomLevel{};
    res.scaledSize = scaledResolution.toThrift();
    res.tileSize = tileSize.toThrift();
    res.tilesUrls = tilesUrls;
    return res;
}
}
}
}