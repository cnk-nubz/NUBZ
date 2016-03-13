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
}
}

#endif