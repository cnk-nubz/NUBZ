#include <unordered_set>

#include <utils/fp_algorithm.h>

#include <repository/Counters.h>
#include <repository/MapImages.h>

#include "InputChecker.h"

namespace server {
namespace utils {

InputChecker::InputChecker(db::DatabaseSession &session) : session(session) {
}

bool InputChecker::checkFrame(std::int32_t floor, std::int32_t x, std::int32_t y,
                              std::int32_t width, std::int32_t height) {
    if (x < 0 || y < 0 || width <= 0 || height <= 0) {
        return false;
    }

    if (auto mapImage = repository::MapImages{session}.get(floor)) {
        return x + width <= mapImage.value().width && y + height <= mapImage.value().height;
    } else {
        return false;
    }
}

bool InputChecker::checkText(const std::string &text) {
    return text.length() &&
           ::utils::all_of(text, [](char c) { return std::isalpha(c) || isspace(c); });
}
}
}