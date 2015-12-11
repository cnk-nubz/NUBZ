#include <boost/filesystem.hpp>

#include "FileHandler.h"

namespace utils {
    FileHandler::FileHandler(const std::string &path) : released(false), path(path) {
    }

    FileHandler::FileHandler(FileHandler &&that) : released(that.released), path(that.path) {
        that.released = true;
    }

    FileHandler::~FileHandler() {
        if (!released) {
            boost::system::error_code ec;
            boost::filesystem::remove(boost::filesystem::path(path), ec);
        }
    }

    void FileHandler::release() noexcept {
        released = true;
    }
}