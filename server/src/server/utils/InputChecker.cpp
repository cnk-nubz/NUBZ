#include <unordered_set>

#include <db/command/GetCounter.h>
#include <db/command/GetMapImages.h>

#include "InputChecker.h"

namespace server {
namespace utils {

InputChecker::InputChecker(db::DatabaseSession &session) : session(session) {
}

bool InputChecker::checkReportId(std::int32_t reportId) {
    return reportId >= 0 &&
           reportId <= db::cmd::GetCounter{db::info::counters::element_type::reports}(session);
}

bool InputChecker::checkFrame(std::int32_t floor, std::int32_t x, std::int32_t y,
                              std::int32_t width, std::int32_t height) {
    if (x < 0 || y < 0 || width <= 0 || height <= 0) {
        return false;
    }

    db::cmd::GetMapImages getMapImage(floor);
    getMapImage(session);
    if (getMapImage.getResult().empty()) {
        return false;
    }

    db::MapImage mapImage = getMapImage.getResult().front();
    return x + width <= mapImage.width && y + height <= mapImage.height;
}

bool InputChecker::checkText(const std::string &text) {
    return text.length() &&
           std::all_of(
               text.begin(), text.end(), [](char c) { return std::isalpha(c) || isspace(c); });
}
}
}