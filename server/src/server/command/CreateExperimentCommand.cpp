#include <utils/fp_algorithm.h>

#include "CreateExperimentCommand.h"

namespace server {
namespace command {

CreateExperimentCommand::CreateExperimentCommand(db::Database &db) : db(db) {
}

void CreateExperimentCommand::operator()(const io::input::CreateExperimentRequest &input) {
    auto lazyExperiment = repository::Experiments::LazyExperiment{};
    lazyExperiment.name = input.name;
    lazyExperiment.actions = input.actions;
    lazyExperiment.breakActions = input.breakActions;
    lazyExperiment.surveyBefore = toRepoSurvey(input.surveyBefore);
    lazyExperiment.surveyAfter = toRepoSurvey(input.surveyAfter);

    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        repo.insert(&lazyExperiment);
    });
}

repository::Experiments::LazyExperiment::Survey CreateExperimentCommand::toRepoSurvey(
    const io::input::QuestionsIdsList &qList) {
    auto res = repository::Experiments::LazyExperiment::Survey{};
    res.simpleQuestions = qList.simpleQuestions;
    res.multipleChoiceQuestions = qList.multipleChoiceQuestions;
    res.sortQuestions = qList.sortQuestions;
    ::utils::transform(qList.questionsOrder, res.typesOrder, [](const io::QuestionType &qType) {
        using QuestionType = repository::Experiments::LazyExperiment::QuestionType;
        switch (qType) {
            case io::QuestionType::Simple:
                return QuestionType::Simple;
            case io::QuestionType::MultipleChoice:
                return QuestionType::MultipleChoice;
            case io::QuestionType::Sort:
                return QuestionType::Sort;
        }
    });
    return res;
}
}
}