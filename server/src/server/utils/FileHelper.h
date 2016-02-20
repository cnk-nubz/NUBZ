#ifndef FILE_HELPER__H
#define FILE_HELPER__H

#include <memory>
#include <string>

#include <boost/filesystem.hpp>

namespace server {
namespace utils {

class FileHelper {
public:
    static void configure(const std::string &tmpDirPath, const std::string &publicDirPath,
                          const std::string &mapTilesDirPath);

    static const FileHelper &getInstance();

    boost::filesystem::path pathForTmpFile(const std::string &filename) const;
    boost::filesystem::path pathForPublicFile(const std::string &filename) const;
    boost::filesystem::path pathForMapTilesDirectory() const;

private:
    FileHelper(const std::string &tmpDirPath, const std::string &publicDirPath,
               const std::string &mapTilesDirPath);

    static std::unique_ptr<FileHelper> instance;

    std::string tmpDirPath;
    std::string publicDirPath;
    std::string mapTilesDirPath;
};
}
}

#endif