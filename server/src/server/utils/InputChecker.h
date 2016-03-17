#ifndef SERVER_UTILS__INPUT_CHECKER__H
#define SERVER_UTILS__INPUT_CHECKER__H

#include <cctype>
#include <string>

#include <utils/fp_algorithm.h>

namespace server {
namespace utils {

inline bool checkText(const std::string &text) {
    return text.length() &&
           ::utils::all_of(text, [](char c) { return std::isalpha(c) || isspace(c); });
}
}
}

#endif