#include <unordered_set>

#include "SaveRawReportCommand.h"
#include "db/command/GetRawReports.h"
#include "db/command/SaveRawReport.h"
#include "io/InvalidInput.h"
#include "InputChecker.h"

namespace command {
    SaveRawReportCommand::SaveRawReportCommand(db::Database &db) : db(db) {
    }

    void SaveRawReportCommand::operator()(const io::input::RawReport &input) {
        db.execute([&](db::DatabaseSession &session) {
            validateReport(session, input);

            // already in database?
            {
                db::cmd::GetRawReports getById(input.reportId);
                getById(session);

                if (!getById.getResult().empty()) {
                    return;
                }
            }

            db::cmd::SaveRawReport saveReport(input.toDb());
            saveReport(session);
        });
    }

    void SaveRawReportCommand::validateReport(db::DatabaseSession &session,
                                              const io::input::RawReport &input) const {
        InputChecker checker(session);

        if (!checker.checkReportId(input.reportId)) {
            throw io::InvalidInput("incorrect report ID");
        }

        std::unordered_set<std::int32_t> exhibitsActions;
        std::unordered_set<std::int32_t> breakActions;
        std::unordered_set<std::int32_t> exhibits;

        for (const auto &event : input.history) {
            if (!event.isBreak()) {
                exhibits.insert(event.getExhibitId());
            }

            std::unordered_set<std::int32_t> &dest =
                event.isBreak() ? breakActions : exhibitsActions;
            for (auto action : event.actions) {
                dest.insert(action);
            }

            if (event.durationInSecs < 0) {
                throw io::InvalidInput("action with duration under 0 seconds?");
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