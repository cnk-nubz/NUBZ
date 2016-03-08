#include <boost/none.hpp>

#include <utils/fp_algorithm.h>

#include <db/table/Experiments.h>

#include "DefaultRepo.h"
#include "Experiments.h"
#include "InvalidData.h"

namespace repository {

using Table = db::table::Experiments;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
Table::Row toDB(const Experiments::LazyExperiment &experiment);
Table::Row::Survey surveyToDB(const Experiments::LazyExperiment::Survey &experiment);

Experiments::Experiment fromDB(db::DatabaseSession &session, const Table::Row &row);

Experiments::LazyExperiment lazyFromDB(const Table::Row &row);
Experiments::LazyExperiment::Survey lazySurveyFromDB(const Table::Row::Survey &survey);
}

Experiments::Experiments(db::DatabaseSession &session) : session(session) {
}

boost::optional<Experiments::Experiment> Experiments::get(std::int32_t ID) {
    if (auto dbExperiment = Impl::get(session, ID)) {
        return fromDB(session, dbExperiment.value());
    } else {
        return {};
    }
}

boost::optional<Experiments::LazyExperiment> Experiments::getLazy(std::int32_t ID) {
    if (auto dbExperiment = Impl::get(session, ID)) {
        return lazyFromDB(dbExperiment.value());
    } else {
        return {};
    }
}

boost::optional<Experiments::Experiment> Experiments::start(std::int32_t ID) {
    assert(!getActive() && "you can only have one active experiment");
    auto sql = Table::update()
                   .where(Table::colId == ID && Table::colState == State::Ready)
                   .set(Table::colState, State::Active)
                   .set(Table::colStartDate, boost::gregorian::day_clock::local_day());
    session.execute(sql);
    return getActive();
}

void Experiments::finishActive() {
    assert(getActive());
    auto sql = Table::update()
                   .where(Table::colState == State::Active)
                   .set(Table::colState, State::Finished)
                   .set(Table::colFinishDate, boost::gregorian::day_clock::local_day());
    session.execute(sql);
}

boost::optional<Experiments::Experiment> Experiments::getActive() {
    auto sql = Table::select().where(Table::colState == State::Active);
    if (auto dbTuple = session.getResult(sql)) {
        return fromDB(session, Table::RowFactory::fromDB(dbTuple.value()));
    } else {
        return {};
    }
}

std::vector<Experiments::Experiment> Experiments::getAllReady() {
    return getAllWithState(State::Ready);
}

std::vector<Experiments::Experiment> Experiments::getAllFinished() {
    return getAllWithState(State::Finished);
}

std::vector<Experiments::Experiment> Experiments::getAllWithState(State state) {
    auto sql = Table::select().where(Table::colState == state);
    auto dbTuples = session.getResults(sql);

    auto result = std::vector<Experiment>{};
    ::utils::transform(dbTuples, result, [&](auto &dbTuple) {
        return fromDB(session, Table::RowFactory::fromDB(dbTuple));
    });
    return result;
}

void Experiments::insert(Experiments::LazyExperiment *experiment) {
    checkActions(*experiment);
    checkActions(*experiment);
    checkSurvey(experiment->surveyBefore);
    checkSurvey(experiment->surveyAfter);

    experiment->startDate = boost::none;
    experiment->finishDate = boost::none;
    experiment->ID = Impl::insert(session, toDB(*experiment));
}

void Experiments::checkActions(const Experiments::LazyExperiment &experiment) {
    auto existing =
        std::unordered_set<std::int32_t>{experiment.actions.begin(), experiment.actions.end()};

    checkIds(existing, experiment.actions);
    checkIds(existing, experiment.breakActions);
}

void Experiments::checkSurvey(const LazyExperiment::Survey &survey) {
    using QuestionType = Experiment::Survey::QuestionType;

    if (utils::count(survey.typesOrder, QuestionType::Simple) != survey.simpleQuestions.size() ||
        utils::count(survey.typesOrder, QuestionType::Sort) != survey.sortQuestions.size() ||
        utils::count(survey.typesOrder, QuestionType::MultipleChoice) !=
            survey.multipleChoiceQuestions.size()) {
        throw InvalidData("types order doesn't match questions");
    }

    using set = std::unordered_set<std::int32_t>;
    auto allSimple = set{survey.simpleQuestions.begin(), survey.simpleQuestions.end()};
    auto allMul = set{survey.multipleChoiceQuestions.begin(), survey.multipleChoiceQuestions.end()};
    auto allSort = set{survey.sortQuestions.begin(), survey.sortQuestions.end()};
    checkIds(allSimple, survey.simpleQuestions);
    checkIds(allMul, survey.multipleChoiceQuestions);
    checkIds(allSort, survey.sortQuestions);
}

void Experiments::checkIds(const std::unordered_set<std::int32_t> &existing,
                           const std::vector<std::int32_t> &choosen) const {
    checkForDuplicates(choosen);
    if (!utils::all_of(choosen,
                       std::bind(&std::unordered_set<std::int32_t>::count, &existing, _1))) {
        throw InvalidData("given id doesn't exist");
    }
}

void Experiments::checkForDuplicates(std::vector<std::int32_t> ids) const {
    std::sort(ids.begin(), ids.end());
    if (std::unique(ids.begin(), ids.end()) != ids.end()) {
        throw InvalidData("duplicated ids");
    }
}

namespace {
Table::Row toDB(const Experiments::LazyExperiment &experiment) {
    auto row = Table::Row{};
    row.name = experiment.name;
    row.actions = experiment.actions;
    row.breakActions = experiment.breakActions;
    row.surveyBefore = surveyToDB(experiment.surveyBefore);
    row.surveyAfter = surveyToDB(experiment.surveyAfter);
    return row;
}

Table::Row::Survey surveyToDB(const Experiments::LazyExperiment::Survey &survey) {
    auto rowSurvey = Table::Row::Survey{};
    rowSurvey.simpleQuestions = survey.simpleQuestions;
    rowSurvey.multipleChoiceQuestions = survey.multipleChoiceQuestions;
    rowSurvey.sortQuestions = survey.sortQuestions;
    ::utils::transform(survey.typesOrder, rowSurvey.typesOrder, [](auto type) { return type; });
    return rowSurvey;
}

Experiments::Experiment fromDB(db::DatabaseSession &session, const Table::Row &row) {
    auto lazy = lazyFromDB(row);
    auto experiment = Experiment{};
    experiment.ID = lazy.ID;
    experiment.name = lazy.name;

    // actions
    {
        auto actionsRepo = Actions{session};
        auto getAction = [&](auto actionId) { return actionsRepo.get(actionId).value(); };
        ::utils::transform(lazy.actions, experiment.actions, getAction);
        ::utils::transform(lazy.breakActions, experiment.breakActions, getAction);
    }

    // surveys
    {
        auto simpleQRepo = SimpleQuestions{session};
        auto sortQRepo = SortQuestions{session};
        auto multiChoiceQRepo = MultipleChoiceQuestions{session};

        auto getSurvey = [&](auto &survey) {
            auto res = Experiment::Survey{};
            res.typesOrder = survey.typesOrder;
            ::utils::transform(survey.simpleQuestions, res.simpleQuestions, [&](auto qId) {
                return simpleQRepo.get(qId).value();
            });
            ::utils::transform(survey.sortQuestions, res.sortQuestions, [&](auto qId) {
                return sortQRepo.get(qId).value();
            });
            ::utils::transform(survey.multipleChoiceQuestions,
                               res.multipleChoiceQuestions,
                               [&](auto qId) { return multiChoiceQRepo.get(qId).value(); });
            return res;
        };

        experiment.surveyBefore = getSurvey(lazy.surveyBefore);
        experiment.surveyAfter = getSurvey(lazy.surveyAfter);
    }

    return experiment;
}

Experiments::LazyExperiment lazyFromDB(const Table::Row &row) {
    auto res = Experiments::LazyExperiment{};
    res.ID = row.ID;
    res.name = row.name;
    res.actions = row.actions;
    res.breakActions = row.breakActions;
    res.surveyBefore = lazySurveyFromDB(row.surveyBefore);
    res.surveyAfter = lazySurveyFromDB(row.surveyAfter);
    return res;
}

Experiments::LazyExperiment::Survey lazySurveyFromDB(const Table::Row::Survey &survey) {
    auto res = Experiments::LazyExperiment::Survey{};
    ::utils::transform(survey.typesOrder, res.typesOrder, [](auto type) {
        return static_cast<Experiments::LazyExperiment::QuestionType>(type);
    });
    res.simpleQuestions = survey.simpleQuestions;
    res.multipleChoiceQuestions = survey.multipleChoiceQuestions;
    res.sortQuestions = survey.sortQuestions;
    return res;
}
}
}