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

template <class T>
void removeDuplicatedIds(std::vector<T> &vec) {
    std::sort(vec.begin(), vec.end());
    auto newEnd = std::unique(vec.begin(), vec.end());
    vec.resize(newEnd - vec.begin());
}
}
}

#endif