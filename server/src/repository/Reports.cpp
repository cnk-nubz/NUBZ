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

namespace repository {

using Table = db::table::Reports;
using Impl = repository::detail::DefaultRepo<Table>;

namespace {
Table::Row toDB(const Reports::Report &report);
Table::Row::Event eventToDB(const Reports::Report::Event &event);
Table::Row::SurveyAns surveyAnsToDB(const Reports::Report::SurveyAns &surveyAns);

Reports::Report fromDB(const Table::Row &report);
Reports::Report::Event eventFromDB(const Table::Row::Event &event);
Reports::Report::SurveyAns surveyAnsFromDB(const Table::Row::SurveyAns &surveyAns);
}

Reports::Reports(db::DatabaseSession &session) : session(session) {
}

bool Reports::exist(std::int32_t ID) {
    auto sql = db::sql::Select<Table::ColumnId>{}.where(Table::colId == ID);
    return (bool)session.getResult(sql);
}

boost::optional<Reports::Report> Reports::get(std::int32_t ID) {
    auto sql = Table::select().where(Table::colId == ID);
    if (auto dbTuple = session.getResult(sql)) {
        return fromDB(Table::RowFactory::fromDB(dbTuple.value()));
    } else {
        return {};
    }
}

std::vector<Reports::Report> Reports::getAllForExperiment(std::int32_t experimentID) {
    auto sql = Table::select().where(Table::colExperimentId == experimentID);
    auto dbTuples = session.getResults(sql);

    auto result = std::vector<Reports::Report>{};
    utils::transform(
        dbTuples, result, [](auto &dbTuple) { return fromDB(Table::RowFactory::fromDB(dbTuple)); });
    return result;
}

void Reports::insert(const Report &report) {
    auto experiment = getExperiment(report.experimentID);
    checkID(report.ID);
    checkExhibits(report.history);
    checkEvents(experiment, report.history);
    checkSurveyAns(experiment.surveyBefore, report.surveyBefore);
    checkSurveyAns(experiment.surveyAfter, report.surveyAfter);

    Impl::insert(session, toDB(report));
}

Experiment Reports::getExperiment(std::int32_t ID) {
    auto experiment = Experiments{session}.get(ID);
    if (experiment) {
        return experiment.value();
    } else {
        throw InvalidData("incorrect experiment ID");
    }
}

void Reports::checkID(std::int32_t ID) {
    if (ID < 0 || ID > Counters{session}.get(CounterType::LastReportID)) {
        throw InvalidData("incorrect report ID");
    }
}

void Reports::checkExhibits(const std::vector<Report::Event> &events) {
    auto allExhibits = Exhibits{session}.getAllIDs();
    auto existing = std::unordered_set<std::int32_t>{allExhibits.begin(), allExhibits.end()};

    for (const auto &event : events) {
        if (event.exhibitID && existing.count(event.exhibitID.value()) == 0) {
            throw InvalidData("incorrect exhibit ID");
        }
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
                throw InvalidData("incorrect exhibit action ID");
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
                throw InvalidData("incorrect break action ID");
            }
        }
    }
}

void Reports::checkDurations(const std::vector<Report::Event> &events) {
    for (const auto &event : events) {
        if (event.durationInSecs < 0) {
            throw InvalidData("durationInSecs cannot be less than 0");
        }
    }
}

void Reports::checkSurveyAns(const Experiment::Survey &survey, const Report::SurveyAns &surveyAns) {
    if (survey.simpleQuestions.size() != surveyAns.simpleQAnswers.size() ||
        survey.sortQuestions.size() != surveyAns.sortQAnswers.size() ||
        survey.multipleChoiceQuestions.size() != surveyAns.multiChoiceQAnswers.size()) {
        throw InvalidData("incorrect number of answers");
    }

    // simple
    {
        auto qIt = survey.simpleQuestions.begin();
        auto qEnd = survey.simpleQuestions.end();
        auto aIt = surveyAns.simpleQAnswers.begin();
        while (qIt != qEnd) {
            checkSimpleAns(*qIt, *aIt);
            ++qIt;
            ++aIt;
        }
    }

    // multiple choice
    {
        auto qIt = survey.multipleChoiceQuestions.begin();
        auto qEnd = survey.multipleChoiceQuestions.end();
        auto aIt = surveyAns.multiChoiceQAnswers.begin();
        while (qIt != qEnd) {
            checkMultipleChoiceAns(*qIt, *aIt);
            ++qIt;
            ++aIt;
        }
    }

    // sort
    {
        auto qIt = survey.sortQuestions.begin();
        auto qEnd = survey.sortQuestions.end();
        auto aIt = surveyAns.sortQAnswers.begin();
        while (qIt != qEnd) {
            checkSortAns(*qIt, *aIt);
            ++qIt;
            ++aIt;
        }
    }
}

void Reports::checkSimpleAns(const SimpleQuestion &question,
                             const Report::SurveyAns::SimpleQAnswer &answer) {
    if (answer) {
        if (question.numberAnswer && !utils::all_of(answer.value(), isdigit)) {
            throw InvalidData(
                "incorrect answer for simple question: number answer should contain only digits");
        }
    }
}

void Reports::checkMultipleChoiceAns(const MultipleChoiceQuestion &question,
                                     const Report::SurveyAns::MultiChoiceQAnswer &answer) {
    if (!answer) {
        return;
    }
    auto available = std::unordered_set<std::int32_t>{};
    for (const auto &opt : question.options) {
        available.insert(opt.ID);
    }

    if (!utils::all_of(answer.value(), std::bind(&decltype(available)::count, &available, _1))) {
        throw InvalidData("incorrect answer for multiple choice question: incorrect id");
    }
}

void Reports::checkSortAns(const SortQuestion &question,
                           const Report::SurveyAns::SortQAnswer &answer) {
    if (!answer) {
        return;
    }
    auto available = std::vector<std::int32_t>{};
    for (const auto &opt : question.options) {
        available.push_back(opt.ID);
    }

    auto choosen = answer.value();
    std::sort(available.begin(), available.end());
    std::sort(choosen.begin(), choosen.end());
    if (available != choosen) {
        throw InvalidData("incorrect answer for sort question");
    }
}

namespace {
Table::Row toDB(const Reports::Report &report) {
    auto res = Table::Row{};
    res.ID = report.ID;
    res.experimentID = report.experimentID;
    utils::transform(report.history, res.history, eventToDB);
    res.surveyBefore = surveyAnsToDB(report.surveyBefore);
    res.surveyAfter = surveyAnsToDB(report.surveyAfter);
    return res;
}

Table::Row::Event eventToDB(const Reports::Report::Event &event) {
    auto res = Table::Row::Event{};
    res.actions = event.actions;
    res.durationInSecs = event.durationInSecs;
    res.exhibitID = event.exhibitID;
    return res;
}

Table::Row::SurveyAns surveyAnsToDB(const Reports::Report::SurveyAns &surveyAns) {
    auto res = Table::Row::SurveyAns{};
    res.simpleQAnswers = surveyAns.simpleQAnswers;
    res.multiChoiceQAnswers = surveyAns.multiChoiceQAnswers;
    res.sortQAnswers = surveyAns.sortQAnswers;
    return res;
}

Reports::Report fromDB(const Table::Row &report) {
    auto res = Reports::Report{};
    res.ID = report.ID;
    res.experimentID = report.experimentID;
    utils::transform(report.history, res.history, eventFromDB);
    res.surveyBefore = surveyAnsFromDB(report.surveyBefore);
    res.surveyAfter = surveyAnsFromDB(report.surveyAfter);
    return res;
}

Reports::Report::Event eventFromDB(const Table::Row::Event &event) {
    auto res = Reports::Report::Event{};
    res.actions = event.actions;
    res.durationInSecs = event.durationInSecs;
    res.exhibitID = event.exhibitID;
    return res;
}

Reports::Report::SurveyAns surveyAnsFromDB(const Table::Row::SurveyAns &surveyAns) {
    auto res = Reports::Report::SurveyAns{};
    res.simpleQAnswers = surveyAns.simpleQAnswers;
    res.multiChoiceQAnswers = surveyAns.multiChoiceQAnswers;
    res.sortQAnswers = surveyAns.sortQAnswers;
    return res;
}
}
}