#include <boost/none.hpp>

#include <repository/MultipleChoicequestions.h>
#include <repository/SimpleQuestions.h>
#include <repository/SortQuestions.h>

#include <db/command/GetCurrentExperiment.h>
#include <db/command/GetExperiments.h>

#include "InputChecker.h"
#include "ReportChecker.h"

namespace server {
namespace utils {

ReportChecker::ReportChecker(db::DatabaseSession &session) : session(session) {
}

bool ReportChecker::loadCurrentExperiment() {
    experiment = db::cmd::GetCurrentExperiment{}(session);
    return (bool)experiment;
}

bool ReportChecker::loadExperiment(std::int32_t experimentId) {
    db::cmd::GetExperiments getExperiments(experimentId);
    getExperiments(session);
    if (!getExperiments.getResult().empty()) {
        experiment = getExperiments.getResult().front();
    } else {
        experiment = boost::none;
    }
    return (bool)experiment;
}

const db::Experiment &ReportChecker::loadedExperiment() const {
    return experiment.value();
}

bool ReportChecker::checkQuestionsBeforeCount(std::size_t simpleQuestions,
                                              std::size_t multipleChoiceQuestions,
                                              std::size_t sortQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(
        loadedExperiment().surveyBefore, simpleQuestions, multipleChoiceQuestions, sortQuestions);
}

bool ReportChecker::checkQuestionsAfterCount(std::size_t simpleQuestions,
                                             std::size_t multipleChoiceQuestions,
                                             std::size_t sortQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(
        loadedExperiment().surveyAfter, simpleQuestions, multipleChoiceQuestions, sortQuestions);
}

bool ReportChecker::checkQuestionsCount(const db::Experiment::Survey &survey,
                                        std::size_t simpleQuestions,
                                        std::size_t multipleChoiceQuestions,
                                        std::size_t sortQuestions) const {
    using QuestionType = db::Experiment::Survey::QuestionType;
    return simpleQuestions == ::utils::count(survey.order, QuestionType::Simple) &&
           multipleChoiceQuestions == ::utils::count(survey.order, QuestionType::MultipleChoice) &&
           sortQuestions == ::utils::count(survey.order, QuestionType::Sort);
}

bool ReportChecker::checkSimpleQuestionAnswer(std::int32_t questionId,
                                              const std::string &answer) const {
    auto repo = repository::SimpleQuestions{session};
    if (auto question = repo.get(questionId)) {
        return (!question.value().numberAnswer && InputChecker::checkText(answer)) ||
               ::utils::all_of(answer, isdigit);
    } else {
        return false;
    }
}

bool ReportChecker::checkMultipleChoiceQuestionAnswer(
    std::int32_t questionId, const std::vector<std::int32_t> &choosenOptions) const {
    auto repo = repository::MultipleChoiceQuestions{session};
    auto options = repo.get(questionId).value().options;
    auto possibilities = std::unordered_set<std::int32_t>{};
    for (const auto &opt : options) {
        possibilities.insert(opt.ID);
    }
    return ::utils::all_of(choosenOptions,
                           std::bind(&decltype(possibilities)::count, &possibilities, _1));
}

bool ReportChecker::checkSortQuestionAnswer(std::int32_t questionId,
                                            const std::vector<std::int32_t> &order) const {
    auto repo = repository::SortQuestions{session};
    auto options = repo.get(questionId).value().options;

    std::unordered_set<std::int32_t> possibilities;
    for (const auto &opt : options) {
        possibilities.insert(opt.ID);
    }
    return order.size() == options.size() &&
           ::utils::all_of(order, std::bind(&decltype(possibilities)::count, &possibilities, _1));
}
}
}