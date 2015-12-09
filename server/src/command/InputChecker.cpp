#include <unordered_set>

#include "InputChecker.h"
#include "db/command/GetCounter.h"

namespace command {
    InputChecker::InputChecker(db::DatabaseSession &session) : session(session) {
    }

    bool InputChecker::checkReportId(std::int32_t reportId) {
        db::cmd::GetCounter cmd(db::info::counters::element_type::reports);
        cmd(session);
        return reportId <= cmd.getResult();
    }
}