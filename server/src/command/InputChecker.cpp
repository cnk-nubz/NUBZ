#include <unordered_set>

#include "InputChecker.h"
#include "db/command/GetCounter.h"
#include "db/command/GetMapImages.h"

namespace command {
    InputChecker::InputChecker(db::DatabaseSession &session) : session(session) {
    }

    bool InputChecker::checkReportId(std::int32_t reportId) {
        db::cmd::GetCounter cmd(db::info::counters::element_type::reports);
        cmd(session);
        return reportId <= cmd.getResult();
    }
    
    bool InputChecker::checkExhibitFrame(std::int32_t floor, std::int32_t x, std::int32_t y,
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
}