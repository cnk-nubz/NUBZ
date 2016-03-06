#include <utils/fp_algorithm.h>

#include <repository/Actions.h>

#include <db/command/GetCurrentExperiment.h>

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

    auto simpleQRepo = repository::SimpleQuestions{session};
    auto sortQRepo = repository::SortQuestions{session};
    auto multiQRepo = repository::MultipleChoiceQuestions{session};

    auto getType = [&](QType qType) {
        switch (qType) {
            case QType::Simple:
                return io::QuestionsList::QuestionType::Simple;
            case QType::Sort:
                return io::QuestionsList::QuestionType::Sort;
            case QType::MultipleChoice:
                return io::QuestionsList::QuestionType::MultipleChoice;
        }
    };

    using namespace io::output;
    auto getSimpleQ = [&](auto id) { return SimpleQuestion{simpleQRepo.get(id).value()}; };
    auto getSortQ = [&](auto id) { return SortQuestion{sortQRepo.get(id).value()}; };
    auto getMultiQ = [&](auto id) { return MultipleChoiceQuestion{multiQRepo.get(id).value()}; };

    ::utils::transform(survey.order, qList.questionsOrder, getType);
    ::utils::transform(survey.simpleQuestions, qList.simpleQuestions, getSimpleQ);
    ::utils::transform(survey.sortQuestions, qList.sortQuestions, getSortQ);
    ::utils::transform(survey.multipleChoiceQuestions, qList.multipleChoiceQuestions, getMultiQ);
}
}
}