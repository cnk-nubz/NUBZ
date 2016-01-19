#include <unordered_set>

#include <db/command/GetRawReports.h>
#include <db/command/SaveRawReport.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "SaveRawReportCommand.h"

namespace server {
namespace command {

SaveRawReportCommand::SaveRawReportCommand(db::Database &db) : db(db) {
}

void SaveRawReportCommand::operator()(const io::input::RawReport &input) {
    db.execute([&](db::DatabaseSession &session) {
        validateReport(session, input);

        // already in database?
        if (!db::cmd::GetRawReports{input.reportId}(session).empty()) {
            return;
        }

        db::cmd::SaveRawReport{utils::toDB(input)}(session);
    });
}

void SaveRawReportCommand::validateReport(db::DatabaseSession &session,
                                          const io::input::RawReport &input) const {
    utils::InputChecker checker(session);

    if (!checker.checkReportId(input.reportId)) {
        throw io::InvalidInput("incorrect report ID");
    }

    std::unordered_set<std::int32_t> exhibitsActions;
    std::unordered_set<std::int32_t> breakActions;
    std::unordered_set<std::int32_t> exhibits;

    for (const auto &event : input.history) {
        if (event.durationInSecs < 0) {
            throw io::InvalidInput("action with duration under 0 seconds?");
        }

        if (event.exhibitId) {
            exhibits.insert(event.exhibitId.value());
        }
        auto &dest = event.exhibitId ? breakActions : exhibitsActions;
        for (auto action : event.actions) {
            dest.insert(action);
        }
    }

    if (!checker.checkExhibitsActionsIds(exhibitsActions)) {
        throw io::InvalidInput("incorrect exhibit action");
    }
    if (!checker.checkBreakActionsIds(breakActions)) {
        throw io::InvalidInput("incorrect break action");
    }
    if (!checker.checkExhibitsIds(exhibits)) {
        throw io::InvalidInput("incorrect exhibit ID");
    }
}
}
}