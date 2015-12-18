#ifndef FILE_HELPER__H
#define FILE_HELPER__H

#include <memory>
#include <string>

#include <boost/filesystem.hpp>

class FileHelper {
public:
    static void configure(const std::string &tmpDirPath, const std::string &publicDirPath);
    static const FileHelper &getInstance();

    boost::filesystem::path pathForTmpFile(const std::string &filename) const;
    boost::filesystem::path pathForPublicFile(const std::string &filename) const;

    std::string pathPrefixForImageTile(std::int32_t floor, std::int32_t level) const;

private:
    FileHelper(const std::string &tmpDirPath, const std::string &publicDirPath);

    static std::unique_ptr<FileHelper> instance;

    std::string tmpDirPath;
    std::string publicDirPath;
};

#endif