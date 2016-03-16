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
Table::Sql::in_t toDB(const Experiments::LazyExperiment &experiment, std::int32_t state);
Table::ContentData::Survey surveyToDB(const Experiments::LazyExperiment::Survey &survey);

Experiments::Experiment fromDB(db::DatabaseSession &session, const Table::Sql::out_t &experiment);

Experiments::LazyExperiment lazyFromDB(const Table::Sql::out_t &experiment);
Experiments::LazyExperiment::Survey lazySurveyFromDB(const Table::ContentData::Survey &survey);
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

void Experiments::start(std::int32_t ID) {
    if (getActive()) {
        throw InvalidData{"you can only have one active experiment"};
    }

    auto sql = Table::Sql::update()
                   .where(Table::ID == ID && Table::State == State::Ready)
                   .set(Table::State, State::Active)
                   .set(Table::StartDate, boost::gregorian::day_clock::local_day());
    session.execute(sql);

    if (!getActive()) {
        throw InvalidData{"there is no ready experiment with given id"};
    }
}

void Experiments::finishActive() {
    if (!getActive()) {
        throw InvalidData{"there is no active experiment"};
    }
    auto sql = Table::Sql::update()
                   .where(Table::State == State::Active)
                   .set(Table::State, State::Finished)
                   .set(Table::FinishDate, boost::gregorian::day_clock::local_day());
    session.execute(sql);
}

boost::optional<Experiments::Experiment> Experiments::getActive() {
    auto sql = Table::Sql::select().where(Table::State == State::Active);
    if (auto dbTuple = session.getResult(sql)) {
        return fromDB(session, dbTuple.value());
    } else {
        return {};
    }
}

boost::optional<Experiments::LazyExperiment> Experiments::getLazyActive() {
    auto sql = Table::Sql::select().where(Table::State == State::Active);
    if (auto dbTuple = session.getResult(sql)) {
        return lazyFromDB(dbTuple.value());
    } else {
        return {};
    }
}

std::vector<Experiments::LazyExperiment> Experiments::getAllReady() {
    return getAllWithState(State::Ready);
}

std::vector<Experiments::LazyExperiment> Experiments::getAllFinished() {
    return getAllWithState(State::Finished);
}

std::vector<Experiments::LazyExperiment> Experiments::getAllWithState(State state) {
    auto sql = Table::Sql::select().where(Table::State == state);

    auto result = std::vector<LazyExperiment>{};
    utils::transform(session.getResults(sql), result, lazyFromDB);
    return result;
}

void Experiments::insert(Experiments::LazyExperiment *experiment) {
    checkActions(*experiment);
    checkSurvey(experiment->surveyBefore);
    checkSurvey(experiment->surveyAfter);

    experiment->startDate = boost::none;
    experiment->finishDate = boost::none;
    experiment->ID = Impl::insert(session, toDB(*experiment, State::Ready));
}

void Experiments::checkActions(const Experiments::LazyExperiment &experiment) {
    auto allActionsIds = Actions{session}.getAllIDs();
    auto existing = std::unordered_set<std::int32_t>{allActionsIds.begin(), allActionsIds.end()};

    checkIds(existing, experiment.actions);
    checkIds(existing, experiment.breakActions);
}

void Experiments::checkSurvey(const LazyExperiment::Survey &survey) {
    using QuestionType = Experiment::Survey::QuestionType;

    if (utils::count(survey.typesOrder, QuestionType::Simple) != survey.simpleQuestions.size() ||
        utils::count(survey.typesOrder, QuestionType::Sort) != survey.sortQuestions.size() ||
        utils::count(survey.typesOrder, QuestionType::MultipleChoice) !=
            survey.multipleChoiceQuestions.size()) {
        throw InvalidData("survey's types order doesn't match questions");
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
Table::Sql::in_t toDB(const Experiments::LazyExperiment &experiment, std::int32_t state) {
    auto content = Table::ContentData{};
    content.actions = experiment.actions;
    content.breakActions = experiment.breakActions;
    content.surveyBefore = surveyToDB(experiment.surveyBefore);
    content.surveyAfter = surveyToDB(experiment.surveyAfter);

    return std::make_tuple(Table::FieldName{experiment.name},
                           Table::FieldState{state},
                           Table::FieldStartDate{},
                           Table::FieldFinishDate{},
                           Table::FieldContent{content});
}

Table::ContentData::Survey surveyToDB(const Experiments::LazyExperiment::Survey &survey) {
    auto dbSurvey = Table::ContentData::Survey{};
    dbSurvey.typesOrder = {survey.typesOrder.begin(), survey.typesOrder.end()};
    dbSurvey.simpleQuestions = survey.simpleQuestions;
    dbSurvey.multipleChoiceQuestions = survey.multipleChoiceQuestions;
    dbSurvey.sortQuestions = survey.sortQuestions;
    return dbSurvey;
}

Experiments::Experiment fromDB(db::DatabaseSession &session, const Table::Sql::out_t &experiment) {
    auto lazy = lazyFromDB(experiment);
    auto res = Experiment{};
    res.ID = lazy.ID;
    res.name = lazy.name;
    res.startDate = lazy.startDate;
    res.finishDate = lazy.finishDate;

    // actions
    {
        auto actionsRepo = Actions{session};
        auto getAction = [&](auto actionId) { return actionsRepo.get(actionId).value(); };
        utils::transform(lazy.actions, res.actions, getAction);
        utils::transform(lazy.breakActions, res.breakActions, getAction);
    }

    // surveys
    {
        auto simpleQRepo = SimpleQuestions{session};
        auto sortQRepo = SortQuestions{session};
        auto multiChoiceQRepo = MultipleChoiceQuestions{session};

        auto getSurvey = [&](auto &survey) {
            auto res = Experiment::Survey{};
            res.typesOrder = survey.typesOrder;
            utils::transform(survey.simpleQuestions, res.simpleQuestions, [&](auto qId) {
                return simpleQRepo.get(qId).value();
            });
            utils::transform(survey.sortQuestions, res.sortQuestions, [&](auto qId) {
                return sortQRepo.get(qId).value();
            });
            utils::transform(survey.multipleChoiceQuestions,
                             res.multipleChoiceQuestions,
                             [&](auto qId) { return multiChoiceQRepo.get(qId).value(); });
            return res;
        };

        res.surveyBefore = getSurvey(lazy.surveyBefore);
        res.surveyAfter = getSurvey(lazy.surveyAfter);
    }

    return res;
}

Experiments::LazyExperiment lazyFromDB(const Table::Sql::out_t &experiment) {
    auto res = Experiments::LazyExperiment{};
    res.ID = std::get<Table::FieldID>(experiment).value;
    res.name = std::get<Table::FieldName>(experiment).value;
    res.startDate = std::get<Table::FieldStartDate>(experiment).value;
    res.finishDate = std::get<Table::FieldFinishDate>(experiment).value;

    auto content = std::get<Table::FieldContent>(experiment).value;
    res.actions = content.actions;
    res.breakActions = content.breakActions;
    res.surveyBefore = lazySurveyFromDB(content.surveyBefore);
    res.surveyAfter = lazySurveyFromDB(content.surveyAfter);
    return res;
}

Experiments::LazyExperiment::Survey lazySurveyFromDB(const Table::ContentData::Survey &survey) {
    auto res = Experiments::LazyExperiment::Survey{};
    utils::transform(survey.typesOrder, res.typesOrder, [](auto type) {
        return static_cast<Experiments::LazyExperiment::QuestionType>(type);
    });
    res.simpleQuestions = survey.simpleQuestions;
    res.multipleChoiceQuestions = survey.multipleChoiceQuestions;
    res.sortQuestions = survey.sortQuestions;
    return res;
}
}
}