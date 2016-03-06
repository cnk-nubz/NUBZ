#include <cassert>

#include "PathHelper.h"

namespace server {
namespace utils {

std::unique_ptr<PathHelper> PathHelper::instance{};

PathHelper &PathHelper::getInstance() {
    if (!instance) {
        PathHelper::instance.reset(new PathHelper);
    }
    return *instance;
}

void PathHelper::setTmpDir(const std::string &tmpDirPath) {
    this->tmpDirPath = tmpDirPath;
}

boost::filesystem::path PathHelper::pathForTmpFile(const std::string &filename) const {
    assert(tmpDirPath);
    auto result = tmpDirPath.value();
    result /= filename;
    return result;
}

void PathHelper::setPublicDir(const std::string &publicDirPath) {
    this->publicDirPath = publicDirPath;
}

boost::filesystem::path PathHelper::pathForPublicFile(const std::string &filename) const {
    assert(publicDirPath);
    auto result = publicDirPath.value();
    result /= filename;
    return result;
}

void PathHelper::setMapTilesDir(const std::string &mapTilesDirPath) {
    this->mapTilesDirPath = mapTilesDirPath;
}

boost::filesystem::path PathHelper::pathForFloorTilesDirectory(std::int32_t floor) const {
    auto res = pathForMapTilesDirectory();
    res /= std::to_string(floor);
    return res;
}

boost::filesystem::path PathHelper::pathForMapTilesDirectory() const {
    assert(mapTilesDirPath);
    return mapTilesDirPath.value();
}

void PathHelper::setMapImgUrlPrefix(const std::string &mapsImgUrlPrefix) {
    this->mapsImgUrlPrefix = mapsImgUrlPrefix;
    if (this->mapsImgUrlPrefix.value().back() != prefixEnd) {
        this->mapsImgUrlPrefix.value() += prefixEnd;
    }
}

std::string PathHelper::mapImgUrlPrefix() const {
    assert(mapsImgUrlPrefix);
    return mapsImgUrlPrefix.value();
}

void PathHelper::setTileUrlPrefix(const std::string &tilesUrlPrefix) {
    this->tilesUrlPrefix = tilesUrlPrefix;
    if (this->tilesUrlPrefix.value().back() != prefixEnd) {
        this->tilesUrlPrefix.value() += prefixEnd;
    }
}

std::string PathHelper::tileUrlPrefix() const {
    assert(tilesUrlPrefix);
    return tilesUrlPrefix.value();
}
}
}