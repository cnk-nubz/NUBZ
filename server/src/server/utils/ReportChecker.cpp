#include <boost/none.hpp>

#include <db/command/GetExperiments.h>
#include <db/command/GetCurrentExperiment.h>
#include <db/command/GetSimpleQuestions.h>
#include <db/command/GetMultipleChoiceQuestions.h>
#include <db/command/GetMultipleChoiceQuestionOptions.h>

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
                                              std::size_t multipleChoiceQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(
        loadedExperiment().surveyBefore, simpleQuestions, multipleChoiceQuestions);
}

bool ReportChecker::checkQuestionsAfterCount(std::size_t simpleQuestions,
                                             std::size_t multipleChoiceQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(
        loadedExperiment().surveyAfter, simpleQuestions, multipleChoiceQuestions);
}

bool ReportChecker::checkQuestionsCount(const db::Experiment::Survey &survey,
                                        std::size_t simpleQuestions,
                                        std::size_t multipleChoiceQuestions) const {
    using QuestionType = db::Experiment::Survey::QuestionType;
    return simpleQuestions == ::utils::count(survey.order, QuestionType::Simple) &&
           multipleChoiceQuestions == ::utils::count(survey.order, QuestionType::MultipleChoice);
}

bool ReportChecker::checkSimpleQuestionAnswer(std::int32_t questionId,
                                              const std::string &answer) const {
    const auto questions = db::cmd::GetSimpleQuestions{questionId}(session);
    if (questions.empty()) {
        return false;
    }
    auto question = questions.front();
    return (!question.numberAnswer && InputChecker::checkText(answer)) ||
           ::utils::all_of(answer, isdigit);
}

bool ReportChecker::checkMultipleChoiceQuestionAnswer(
    std::int32_t questionId, const std::vector<std::int32_t> &choosenOptions) const {
    const auto options = db::cmd::GetMultipleChoiceQuestionOptions{questionId}(session);
    std::unordered_set<std::int32_t> possibilities;
    for (const auto &opt : options) {
        possibilities.insert(opt.ID);
    }
    return ::utils::all_of(choosenOptions,
                           std::bind(&decltype(possibilities)::count, &possibilities, _1));
}
}
}