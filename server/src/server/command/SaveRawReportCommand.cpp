#include <vector>

#include <db/command/GetRawReports.h>
#include <db/command/SaveRawReport.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/ReportChecker.h>
#include <server/utils/io_translation.h>

#include "SaveRawReportCommand.h"

namespace server {
namespace command {

SaveRawReportCommand::SaveRawReportCommand(db::Database &db) : db(db) {
}

void SaveRawReportCommand::operator()(const io::input::RawReport &input) {
    db.execute([&](db::DatabaseSession &session) {
        validateReport(session, input);
        auto clearInput = removeDuplicatedIds(input);

        // already in database?
        if (!db::cmd::GetRawReports{clearInput.ID}(session).empty()) {
            return;
        }

        db::cmd::SaveRawReport{utils::toDB(clearInput)}(session);
    });
}

io::input::RawReport SaveRawReportCommand::removeDuplicatedIds(io::input::RawReport input) const {
    for (auto &event : input.history) {
        std::sort(event.actions.begin(), event.actions.end());
        auto newEnd = std::unique(event.actions.begin(), event.actions.end());
        event.actions.resize(newEnd - event.actions.begin());
    }
    return input;
}

void SaveRawReportCommand::validateReport(db::DatabaseSession &session,
                                          const io::input::RawReport &input) const {
    utils::InputChecker checker(session);
    utils::ReportChecker reportChecker(session);

    if (!checker.checkReportId(input.ID)) {
        throw io::InvalidInput("incorrect report ID");
    }
    if (!reportChecker.loadExperiment(input.experimentId)) {
        throw io::InvalidInput("incorrect experiment ID");
    }

    std::vector<std::int32_t> exhibitsActions;
    std::vector<std::int32_t> breakActions;
    std::vector<std::int32_t> exhibits;

    for (const auto &event : input.history) {
        if (event.durationInSecs < 0) {
            throw io::InvalidInput("action with duration under 0 seconds?");
        }

        auto &dest = event.exhibitId ? exhibitsActions : breakActions;
        for (auto action : event.actions) {
            dest.push_back(action);
        }
        if (event.exhibitId) {
            exhibits.push_back(event.exhibitId.value());
        }
    }

    if (!reportChecker.checkExhibitActionsIds(exhibitsActions)) {
        throw io::InvalidInput("incorrect exhibit action");
    }
    if (!reportChecker.checkBreakActionsIds(breakActions)) {
        throw io::InvalidInput("incorrect break action");
    }
    if (!reportChecker.checkQuestionsBeforeCount(input.simpleQuestionsAnswersBefore.size())) {
        throw io::InvalidInput("incorrect number of questions answer in survey before");
    }
    if (!reportChecker.checkQuestionsBeforeCount(input.simpleQuestionsAnswersBefore.size())) {
        throw io::InvalidInput("incorrect number of questions answer in survey after");
    }
    if (!checker.checkExhibitsIds(exhibits)) {
        throw io::InvalidInput("incorrect exhibit ID");
    }
}
}
}