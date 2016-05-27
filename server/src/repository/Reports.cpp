#include <unordered_set>

#include <utils/fp_algorithm.h>

#include <db/table/Reports.h>

#include "Actions.h"
#include "Counters.h"
#include "DefaultRepo.h"
#include "Exhibits.h"
#include "MultipleChoiceQuestions.h"
#include "Reports.h"
#include "SimpleQuestions.h"
#include "SortQuestions.h"
#include "error/InvalidData.h"

namespace repository {

using Table = db::table::Reports;
using Impl = repository::detail::DefaultRepo<Table>;

namespace {
Table::Sql::in_t toDB(const Reports::Report &report);
Table::ContentData::Time timeToDB(const utils::TimePoint &time);
Table::ContentData::Event eventToDB(const Reports::Report::Event &event);
Table::ContentData::SurveyAns surveyAnsToDB(const Reports::Report::SurveyAns &surveyAns);

Reports::Report fromDB(const Table::Sql::out_t &report);
utils::TimePoint timeFromDB(const Table::ContentData::Time &time);
Reports::Report::Event eventFromDB(const Table::ContentData::Event &event);
Reports::Report::SurveyAns surveyAnsFromDB(const Table::ContentData::SurveyAns &surveyAns);
}

Reports::Reports(db::DatabaseSession &session) : session(session) {
}

bool Reports::exist(std::int32_t ID) {
    auto sql = db::sql::Select<Table::FieldID>{}.where(Table::ID == ID);
    return (bool)session.getResult(sql);
}

Reports::Report Reports::getF(std::int32_t ID) {
    if (auto report = get(ID)) {
        return report.value();
    } else {
        throw InvalidData{"incorrect report ID"};
    }
}

boost::optional<Reports::Report> Reports::get(std::int32_t ID) {
    auto sql = Table::Sql::select().where(Table::ID == ID);
    if (auto dbTuple = session.getResult(sql)) {
        return fromDB(dbTuple.value());
    } else {
        return {};
    }
}

std::vector<Reports::ReportInfo> Reports::getAllInfosForExperiment(std::int32_t experimentID) {
    auto sql =
        db::sql::Select<Table::FieldID, Table::FieldReceiveDate, Table::FieldExperimentID>{}.where(
            Table::ExperimentID == experimentID);

    auto result = std::vector<Reports::ReportInfo>{};
    for (const auto &dbOut : session.getResults(sql)) {
        auto info = ReportInfo{};
        info.ID = std::get<Table::FieldID>(dbOut).value;
        info.receiveDate = std::get<Table::FieldReceiveDate>(dbOut).value;
        result.push_back(info);
    }
    return result;
}

std::vector<Reports::Report> Reports::getAllForExperiment(std::int32_t experimentID) {
    auto sql = Table::Sql::select().where(Table::ExperimentID == experimentID);
    auto result = std::vector<Reports::Report>{};
    utils::transform(session.getResults(sql), result, fromDB);
    return result;
}

void Reports::insert(const Report &report) {
    auto experimentOpt = Experiments{session}.getActive();
    if (!experimentOpt || experimentOpt.value().ID != report.experimentID) {
        throw InvalidData{"given experiment ID is not an ID of currently active experiment"};
    }
    auto experiment = experimentOpt.value();

    checkID(report.ID);
    checkEvents(experiment, report.history);
    checkSurveyAns(experiment.surveyBefore, report.surveyBefore);
    checkSurveyAns(experiment.surveyAfter, report.surveyAfter);

    retainExhibits(report.history);
    Impl::insert(session, toDB(report));
}

void Reports::checkID(std::int32_t ID) {
    if (ID < 0 || ID > Counters{session}.get(CounterType::LastReportID)) {
        throw InvalidData{"incorrect report ID"};
    }
}

void Reports::checkEvents(const Experiment &experiment, const std::vector<Report::Event> &events) {
    checkActions(experiment, events);
    checkBreakActions(experiment, events);
    checkDurations(events);
}

void Reports::checkActions(const Experiment &experiment, const std::vector<Report::Event> &events) {
    auto available = std::unordered_set<std::int32_t>{};
    for (const auto &action : experiment.actions) {
        available.insert(action.ID);
    }

    for (const auto &event : events) {
        if (event.exhibitID) {
            if (!utils::all_of(event.actions,
                               std::bind(&decltype(available)::count, &available, _1))) {
                throw InvalidData{"incorrect exhibit action ID"};
            }
        }
    }
}

void Reports::checkBreakActions(const Experiment &experiment,
                                const std::vector<Report::Event> &events) {
    auto available = std::unordered_set<std::int32_t>{};
    for (const auto &action : experiment.breakActions) {
        available.insert(action.ID);
    }

    for (const auto &event : events) {
        if (!event.exhibitID) {
            if (!utils::all_of(event.actions,
                               std::bind(&decltype(available)::count, &available, _1))) {
                throw InvalidData{"incorrect break action ID"};
            }
        }
    }
}

void Reports::checkDurations(const std::vector<Report::Event> &events) const {
    for (const auto &event : events) {
        if (event.durationInSecs < 0) {
            throw InvalidData{"durationInSecs cannot be less than 0"};
        }

        checkTimePoint(event.beginTime);
    }
}

void Reports::checkTimePoint(const utils::TimePoint &timePoint) const {
    if (timePoint.h < 0 || timePoint.m < 0 || timePoint.s < 0 || timePoint.h > 23 ||
        timePoint.m > 59 || timePoint.s > 59) {
        throw InvalidData{"incorrect time"};
    }
}

void Reports::checkSurveyAns(const Experiment::Survey &survey, const Report::SurveyAns &surveyAns) {
    if (survey.simpleQuestions.size() != surveyAns.simpleQAnswers.size() ||
        survey.sortQuestions.size() != surveyAns.sortQAnswers.size() ||
        survey.multipleChoiceQuestions.size() != surveyAns.multiChoiceQAnswers.size()) {
        throw InvalidData{"incorrect number of answers"};
    }

    utils::for_each2(survey.simpleQuestions, surveyAns.simpleQAnswers, checkSimpleAns);
    utils::for_each2(
        survey.multipleChoiceQuestions, surveyAns.multiChoiceQAnswers, checkMultipleChoiceAns);
    utils::for_each2(survey.sortQuestions, surveyAns.sortQAnswers, checkSortAns);
}

void Reports::checkSimpleAns(const SimpleQuestion &question,
                             const Report::SurveyAns::SimpleQAnswer &answer) {
    if (answer.empty()) {
        throw InvalidData{"answer for simple question cannot be empty"};
    }
    if (question.numberAnswer && !utils::all_of(answer, isdigit)) {
        throw InvalidData{
            "incorrect answer for simple question: number answer should contain only digits"};
    }
}

void Reports::checkMultipleChoiceAns(const MultipleChoiceQuestion &question,
                                     const Report::SurveyAns::MultiChoiceQAnswer &answer) {
    if (answer.empty()) {
        throw InvalidData{"answer for multiple choice question cannot be empty"};
    }

    auto available = std::unordered_set<std::int32_t>{};
    for (const auto &opt : question.options) {
        available.insert(opt.ID);
    }

    if (!utils::all_of(answer, std::bind(&decltype(available)::count, &available, _1))) {
        throw InvalidData{"incorrect answer for multiple choice question: incorrect id"};
    }
}

void Reports::checkSortAns(const SortQuestion &question,
                           const Report::SurveyAns::SortQAnswer &answer) {
    auto available = std::vector<std::int32_t>{};
    for (const auto &opt : question.options) {
        available.push_back(opt.ID);
    }

    auto choosen = answer;
    std::sort(available.begin(), available.end());
    std::sort(choosen.begin(), choosen.end());
    if (available != choosen) {
        throw InvalidData{"incorrect answer for sort question"};
    }
}

void Reports::retainExhibits(const std::vector<Report::Event> &events) {
    auto exhibitsRepo = Exhibits{session};
    for (const auto &event : events) {
        if (event.exhibitID) {
            exhibitsRepo.incReferenceCount(event.exhibitID.value());
        }
    }
}

void Reports::releaseExhibits(const std::vector<Report::Event> &events) {
    auto exhibitsRepo = Exhibits{session};
    for (const auto &event : events) {
        if (event.exhibitID) {
            exhibitsRepo.decReferenceCount(event.exhibitID.value());
        }
    }
}

namespace {
Table::Sql::in_t toDB(const Reports::Report &report) {
    auto content = Table::ContentData{};
    content.beginTime = timeToDB(report.beginTime);
    content.finishTime = timeToDB(report.finishTime);
    utils::transform(report.history, content.history, eventToDB);
    content.surveyBefore = surveyAnsToDB(report.surveyBefore);
    content.surveyAfter = surveyAnsToDB(report.surveyAfter);

    return std::make_tuple(Table::FieldID{report.ID},
                           Table::FieldReceiveDate{boost::gregorian::day_clock::local_day()},
                           Table::FieldExperimentID{report.experimentID},
                           Table::FieldContent{content});
}

Table::ContentData::Event eventToDB(const Reports::Report::Event &event) {
    auto res = Table::ContentData::Event{};
    res.actions = event.actions;
    res.beginTime = timeToDB(event.beginTime);
    res.durationInSecs = event.durationInSecs;
    res.exhibitID = event.exhibitID;
    return res;
}

Table::ContentData::Time timeToDB(const utils::TimePoint &time) {
    auto res = Table::ContentData::Time{};
    res.h = time.h;
    res.m = time.m;
    res.s = time.s;
    return res;
}

Table::ContentData::SurveyAns surveyAnsToDB(const Reports::Report::SurveyAns &surveyAns) {
    auto res = Table::ContentData::SurveyAns{};
    res.simpleQAnswers = surveyAns.simpleQAnswers;
    res.multiChoiceQAnswers = surveyAns.multiChoiceQAnswers;
    res.sortQAnswers = surveyAns.sortQAnswers;
    return res;
}

Reports::Report fromDB(const Table::Sql::out_t &report) {
    auto res = Reports::Report{};
    res.ID = std::get<Table::FieldID>(report).value;
    res.receiveDate = std::get<Table::FieldReceiveDate>(report).value;
    res.experimentID = std::get<Table::FieldExperimentID>(report).value;

    auto content = std::get<Table::FieldContent>(report).value;
    res.beginTime = timeFromDB(content.beginTime);
    res.finishTime = timeFromDB(content.finishTime);
    utils::transform(content.history, res.history, eventFromDB);
    res.surveyBefore = surveyAnsFromDB(content.surveyBefore);
    res.surveyAfter = surveyAnsFromDB(content.surveyAfter);
    return res;
}

Reports::Report::Event eventFromDB(const Table::ContentData::Event &event) {
    auto res = Reports::Report::Event{};
    res.actions = event.actions;
    res.beginTime = timeFromDB(event.beginTime);
    res.durationInSecs = event.durationInSecs;
    res.exhibitID = event.exhibitID;
    return res;
}

utils::TimePoint timeFromDB(const Table::ContentData::Time &time) {
    auto res = utils::TimePoint{};
    res.h = time.h;
    res.m = time.m;
    res.s = time.s;
    return res;
}

Reports::Report::SurveyAns surveyAnsFromDB(const Table::ContentData::SurveyAns &surveyAns) {
    auto res = Reports::Report::SurveyAns{};
    res.simpleQAnswers = surveyAns.simpleQAnswers;
    res.multiChoiceQAnswers = surveyAns.multiChoiceQAnswers;
    res.sortQAnswers = surveyAns.sortQAnswers;
    return res;
}
}
}