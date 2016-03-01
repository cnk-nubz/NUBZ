#include <utils/fp_algorithm.h>

#include <repository/Actions.h>

#include <db/command/GetCurrentExperiment.h>
#include <db/command/GetSimpleQuestions.h>
#include <db/command/GetMultipleChoiceQuestions.h>
#include <db/command/GetMultipleChoiceQuestionOptions.h>
#include <db/command/GetSortQuestions.h>
#include <db/command/GetSortQuestionOptions.h>

#include <server/utils/io_translation.h>

#include "GetCurrentExperimentCommand.h"

namespace server {
namespace command {

GetCurrentExperimentCommand::GetCurrentExperimentCommand(db::Database &db) : db(db) {
}

io::output::CurrentExperimentResponse GetCurrentExperimentCommand::operator()() {
    io::output::CurrentExperimentResponse response;
    db.execute([&](db::DatabaseSession &session) {
        boost::optional<db::Experiment> experimentOpt = db::cmd::GetCurrentExperiment{}(session);
        if (!experimentOpt) {
            return;
        }

        auto experiment = experimentOpt.value();
        currentExperiment.ID = experiment.ID;
        currentExperiment.name = experiment.name;
        fillExperimentData(experiment, session);

        response.experiment = currentExperiment;
    });

    return response;
}

void GetCurrentExperimentCommand::fillExperimentData(const db::Experiment &experiment,
                                                     db::DatabaseSession &session) {
    auto actionsRepo = repository::Actions{session};
    auto getAction = [&](auto id) { return actionsRepo.get(id).value(); };

    ::utils::transform(experiment.actions, currentExperiment.exhibitActions, getAction);
    ::utils::transform(experiment.breakActions, currentExperiment.breakActions, getAction);
    fillSurvey(experiment.surveyBefore, currentExperiment.surveyBefore, session);
    fillSurvey(experiment.surveyAfter, currentExperiment.surveyAfter, session);
}

void GetCurrentExperimentCommand::fillSurvey(const db::Experiment::Survey &survey,
                                             io::QuestionsList &qList,
                                             db::DatabaseSession &session) {
    using namespace db::cmd;
    using namespace utils;
    using QType = db::Experiment::Survey::QuestionType;

    auto getType = [&](QType qType) {
        switch (qType) {
            case QType::Simple:
                return io::QuestionsList::QuestionType::Simple;
            case QType::MultipleChoice:
                return io::QuestionsList::QuestionType::MultipleChoice;
            case QType::Sort:
                return io::QuestionsList::QuestionType::Sort;
        }
    };
    auto getSimpleQ = [&](std::int32_t id) {
        return toIO(GetSimpleQuestions{id}(session).front());
    };
    auto getMultiQ = [&](std::int32_t id) {
        return toIO(GetMultipleChoiceQuestions{id}(session).front(),
                    GetMultipleChoiceQuestionOptions{id}(session));
    };
    auto getSortQ = [&](std::int32_t id) {
        return toIO(GetSortQuestions{id}(session).front(), GetSortQuestionOptions{id}(session));
    };

    ::utils::transform(survey.order, qList.questionsOrder, getType);
    ::utils::transform(survey.simpleQuestions, qList.simpleQuestions, getSimpleQ);
    ::utils::transform(survey.multipleChoiceQuestions, qList.multipleChoiceQuestions, getMultiQ);
    ::utils::transform(survey.sortQuestions, qList.sortQuestions, getSortQ);
}
}
}