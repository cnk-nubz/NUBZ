#include <cassert>

#include "FileHelper.h"

std::unique_ptr<FileHelper> FileHelper::instance{};

void FileHelper::configure(const std::string &tmpDirPath, const std::string &publicDirPath) {
    FileHelper::instance.reset(new FileHelper(tmpDirPath, publicDirPath));
}

const FileHelper &FileHelper::getInstance() {
    assert(instance);
    return *instance;
}

FileHelper::FileHelper(const std::string &tmpDirPath, const std::string &publicDirPath)
    : tmpDirPath(tmpDirPath), publicDirPath(publicDirPath) {
}

boost::filesystem::path FileHelper::pathForTmpFile(const std::string &filename) const {
    namespace fs = boost::filesystem;
    fs::path result(tmpDirPath);
    result /= filename;
    return result;
}

boost::filesystem::path FileHelper::pathForPublicFile(const std::string &filename) const {
    namespace fs = boost::filesystem;
    fs::path result(publicDirPath);
    result /= filename;
    return result;
}