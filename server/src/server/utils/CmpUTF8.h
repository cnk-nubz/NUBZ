#ifndef SERVER_UTILS__CMP_UTF8__H
#define SERVER_UTILS__CMP_UTF8__H

#include <locale>
#include <string>

namespace server {
namespace utils {

inline bool cmpUTF8(const std::string &lhs, const std::string &rhs) {
    static const auto comparator = std::locale{"pl_PL.UTF-8"};
    return comparator(lhs, rhs);
}
}
}

#endif