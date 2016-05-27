#ifndef UTILS__LOG__H
#define UTILS__LOG__H

#include <iostream>
#include <map>
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

template <class K, class V, class C>
std::ostream &operator<<(std::ostream &stream, const std::map<K, V, C> &map) {
    stream << "{";
    auto it = map.begin();
    while (it != map.end()) {
        stream << it->first << ": " << it->second;
        if (++it != map.end()) {
            stream << ", ";
        }
    }
    stream << "}";
    return stream;
}
}

#define ELPP_THREAD_SAFE 1

#include <easylogging++.h>

#endif