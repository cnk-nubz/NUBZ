#include <cassert>

#include "PathHelper.h"

namespace server {
namespace utils {

PathHelper::Dir PathHelper::tmpDir{};
PathHelper::Dir PathHelper::publicDir{};
PathHelper::Dir PathHelper::mapTilesDir{};

PathHelper::Url PathHelper::mapsImgUrl;
PathHelper::Url PathHelper::tilesUrl;

void PathHelper::Dir::setPath(const std::string &path) {
    this->path = path;
}

boost::filesystem::path PathHelper::Dir::pathForFile(const std::string &filename) const {
    assert(path);
    auto res = path.value() / filename;
    return res;
}

void PathHelper::Url::setPrefix(const std::string url) {
    urlPrefix = std::move(url);
    if (urlPrefix.value().back() != prefixEnd) {
        urlPrefix.value() += prefixEnd;
    }
}

std::string PathHelper::Url::urlFor(const std::string &filename) const {
    assert(urlPrefix);
    return urlPrefix.value() + filename;
}

boost::filesystem::path PathHelper::pathForFloorTilesDirectory(std::int32_t floor) {
    return mapTilesDir.pathForFile(std::to_string(floor));
}
}
}