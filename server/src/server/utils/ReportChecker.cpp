#include <boost/none.hpp>

#include <db/command/GetExperiments.h>
#include <db/command/GetCurrentExperiment.h>

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

bool ReportChecker::checkQuestionsBeforeCount(std::size_t simpleQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(experiment.value().surveyBefore, simpleQuestions);
}

bool ReportChecker::checkQuestionsAfterCount(std::size_t simpleQuestions) const {
    if (!experiment) {
        return false;
    }
    return checkQuestionsCount(experiment.value().surveyAfter, simpleQuestions);
}

bool ReportChecker::checkQuestionsCount(const db::Experiment::Survey &survey,
                                        std::size_t simpleQuestions) const {
    static const auto Simple = db::Experiment::Survey::QuestionType::Simple;
    return simpleQuestions == ::utils::count(survey.order, Simple);
}
}
}