#ifndef UTILS__LOG__H
#define UTILS__LOG__H

namespace communication {
template <class T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec) {
    stream << "[";
    auto it = vec.begin();
    while (true) {
        stream << *it;
        if (++it != vec.end()) {
            stream << ", ";
        } else {
            break;
        }
    }
    stream << "]";
    return stream;
}
}

#include <external/easylogging++.h>

#endif