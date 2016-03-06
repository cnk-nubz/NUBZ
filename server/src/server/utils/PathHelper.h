#ifndef PATH_HELPER__H
#define PATH_HELPER__H

#include <memory>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

namespace server {
namespace utils {

class PathHelper {
public:
    static PathHelper &getInstance();

    void setTmpDir(const std::string &tmpDirPath);
    boost::filesystem::path pathForTmpFile(const std::string &filename) const;

    void setPublicDir(const std::string &publicDirPath);
    boost::filesystem::path pathForPublicFile(const std::string &filename) const;

    void setMapTilesDir(const std::string &mapTilesDirPath);
    boost::filesystem::path pathForMapTilesDirectory() const;

    boost::filesystem::path pathForFloorTilesDirectory(std::int32_t floor) const;

    void setMapImgUrlPrefix(const std::string &mapsImgUrlPrefix);
    std::string mapImgUrlPrefix() const;

    void setTileUrlPrefix(const std::string &tilesUrlPrefix);
    std::string tileUrlPrefix() const;

private:
    PathHelper() = default;

    static std::unique_ptr<PathHelper> instance;
    static constexpr char prefixEnd = '/';

    boost::optional<boost::filesystem::path> tmpDirPath;
    boost::optional<boost::filesystem::path> publicDirPath;
    boost::optional<boost::filesystem::path> mapTilesDirPath;
    boost::optional<std::string> mapsImgUrlPrefix;
    boost::optional<std::string> tilesUrlPrefix;
};
}
}

#endif