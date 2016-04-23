#ifndef SERVER_UTILS__PATH_HELPER__H
#define SERVER_UTILS__PATH_HELPER__H

#include <memory>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

namespace server {
namespace utils {

struct PathHelper {
    class Dir {
    public:
        void setPath(const std::string &path);
        boost::filesystem::path pathForFile(const std::string &filename) const;

    private:
        boost::optional<boost::filesystem::path> path;
    };

    class Url {
    public:
        void setPrefix(const std::string url);
        std::string urlFor(const std::string &filename) const;

    private:
        static constexpr char prefixEnd = '/';
        boost::optional<std::string> urlPrefix;
    };

    PathHelper() = delete;

    static Dir tmpDir;
    static Dir publicDir;
    static Dir mapTilesDir;
    static Dir excelReportsDir;

    static Url mapsImgUrl;
    static Url tilesUrl;

    static boost::filesystem::path pathForFloorTilesDirectory(std::int32_t floor);

    static boost::filesystem::path pathForExcelZip(std::int32_t experimentID,
                                                   std::int32_t reportID);
    static boost::filesystem::path pathForExcelZip(std::int32_t experimentID);
};
}
}

#endif