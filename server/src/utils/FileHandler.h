#ifndef UTILS__FILE_HANDLER__H
#define UTILS__FILE_HANDLER__H

#include <string>

namespace utils {
    class FileHandler {
    public:
        FileHandler(const std::string &path);
        ~FileHandler();
        FileHandler(const FileHandler &) = delete;
        FileHandler(FileHandler &&);
        FileHandler &operator=(const FileHandler &) = delete;
        FileHandler &operator=(FileHandler &&) = delete;

        const std::string &getPath() const noexcept;
        void release() noexcept;

    private:
        bool released;
        const std::string path;
    };
}

#endif