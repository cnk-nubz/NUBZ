#ifndef UTILS__LOG__H
#define UTILS__LOG__H

#include <iostream>
#include <vector>

namespace communication {
template <class T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec) {
    stream << "[";
    auto it = vec.begin();
    while (it != vec.end()) {
        stream << *it;
        if (++it != vec.end()) {
            stream << ", ";
        }
    }
    stream << "]";
    return stream;
}
}

#include <easylogging++.h>

#endif