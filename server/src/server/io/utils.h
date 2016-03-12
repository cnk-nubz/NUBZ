#ifndef SERVER_IO__UTILS__H
#define SERVER_IO__UTILS__H

#include <vector>

#include <utils/fp_algorithm.h>

namespace server {
namespace io {

template <class T>
std::vector<typename T::thrift_t> ioToThrift(const std::vector<T> &io) {
    std::vector<typename T::thrift_t> res;
    ::utils::transform(io, res, [](const auto &e) { return e.toThrift(); });
    return res;
}

template <class IOType>
std::vector<IOType> repoToIO(const std::vector<typename IOType::repo_t> &repo) {
    std::vector<IOType> res;
    for (const auto &r : repo) {
        res.emplace_back(r);
    }
    return res;
}
}
}

#endif