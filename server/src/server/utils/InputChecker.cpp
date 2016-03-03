#include <unordered_set>

#include <utils/fp_algorithm.h>

#include <repository/Counters.h>

#include <db/command/GetMapImages.h>

#include "InputChecker.h"

namespace server {
namespace utils {

InputChecker::InputChecker(db::DatabaseSession &session) : session(session) {
}

bool InputChecker::checkReportId(std::int32_t reportId) {
    auto repo = repository::Counters{session};
    return reportId >= 0 && reportId <= repo.get(repository::CounterType::LastReportID);
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
           ::utils::all_of(text, [](char c) { return std::isalpha(c) || isspace(c); });
}
}
}