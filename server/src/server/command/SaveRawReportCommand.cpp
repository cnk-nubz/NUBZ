#include <vector>

#include <db/command/GetRawReports.h>
#include <db/command/SaveRawReport.h>

#include <server/io/InvalidInput.h>
#include <server/io/utils.h>
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
        server::io::removeDuplicatedIds(event.actions);
    }
    for (auto *answers : {&input.answersBefore, &input.answersAfter}) {
        for (auto &multipleChoiceAnswer : answers->multipleChoiceQuestionsAnswers) {
            if (multipleChoiceAnswer.choosenOptions) {
                server::io::removeDuplicatedIds(multipleChoiceAnswer.choosenOptions.value());
            }
        }
    }
    return input;
}

void SaveRawReportCommand::validateReport(db::DatabaseSession &session,
                                          const io::input::RawReport &input) const {
    utils::InputChecker checker(session);
    //    utils::ReportChecker reportChecker(session);

    if (!checker.checkReportId(input.ID)) {
        throw io::InvalidInput("incorrect report ID");
    }
    //    if (!reportChecker.loadExperiment(input.experimentId)) {
    //        throw io::InvalidInput("incorrect experiment ID");
    //    }

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

    //    if (!reportChecker.checkExhibitActionsIds(exhibitsActions)) {
    //        throw io::InvalidInput("incorrect exhibit action");
    //    }
    //    if (!reportChecker.checkBreakActionsIds(breakActions)) {
    //        throw io::InvalidInput("incorrect break action");
    //    }
    //    if (!reportChecker.checkQuestionsBeforeCount(
    //            input.answersBefore.simpleQuestionsAnswers.size(),
    //            input.answersBefore.multipleChoiceQuestionsAnswers.size(),
    //            input.answersBefore.sortQuestionsAnswers.size())) {
    //        throw io::InvalidInput("incorrect number of questions answers in survey before");
    //    }
    //    if (!reportChecker.checkQuestionsBeforeCount(
    //            input.answersAfter.simpleQuestionsAnswers.size(),
    //            input.answersAfter.multipleChoiceQuestionsAnswers.size(),
    //            input.answersAfter.sortQuestionsAnswers.size())) {
    //        throw io::InvalidInput("incorrect number of questions answers in survey after");
    //    }

    //    validateAnswers(
    //        reportChecker, input.answersBefore, reportChecker.loadedExperiment().surveyBefore);
    //    validateAnswers(
    //        reportChecker, input.answersAfter, reportChecker.loadedExperiment().surveyAfter);

    if (!checker.checkExhibitsIds(exhibits)) {
        throw io::InvalidInput("incorrect exhibit ID");
    }
}

// void SaveRawReportCommand::validateAnswers(const utils::ReportChecker &reportChecker,
//                                           const io::input::RawReport::SurveyAnswers &answers,
//                                           const db::Experiment::Survey &survey) const {
//    {
//        auto questionIt = survey.simpleQuestions.begin();
//        for (const auto &ans : answers.simpleQuestionsAnswers) {
//            if (ans.answer &&
//                !reportChecker.checkSimpleQuestionAnswer(*questionIt, ans.answer.value())) {
//                throw io::InvalidInput("incorrect answer for simple question");
//            }
//            ++questionIt;
//        }
//    }
//
//    {
//        auto questionIt = survey.multipleChoiceQuestions.begin();
//        for (const auto &ans : answers.multipleChoiceQuestionsAnswers) {
//            const auto &options = ans.choosenOptions;
//            if (options &&
//                !reportChecker.checkMultipleChoiceQuestionAnswer(*questionIt, options.value())) {
//                throw io::InvalidInput("incorrect answer for multiple choice question");
//            }
//            ++questionIt;
//        }
//    }
//    {
//        auto questionIt = survey.sortQuestions.begin();
//        for (const auto &ans : answers.sortQuestionsAnswers) {
//            const auto &options = ans.choosenOrder;
//            if (options && !reportChecker.checkSortQuestionAnswer(*questionIt, options.value())) {
//                throw io::InvalidInput("incorrect answer for sort question");
//            }
//            ++questionIt;
//        }
//    }
//}
}
}