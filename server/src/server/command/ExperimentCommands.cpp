#include <algorithm>

#include <server/io/InvalidInput.h>

#include "ExperimentCommands.h"

namespace server {
namespace command {

ExperimentCommands::ExperimentCommands(db::Database &db) : db(db) {
}

void ExperimentCommands::create(const CreateExperimentRequest &input) {
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

void ExperimentCommands::finish() {
    db.execute(
        [](db::DatabaseSession &session) { repository::Experiments{session}.finishActive(); });
}

void ExperimentCommands::start(std::int32_t ID) {
    db.execute([&](db::DatabaseSession &session) { repository::Experiments{session}.start(ID); });
}

SingleExperimentInfo ExperimentCommands::getActive() {
    auto repoExp = db.execute([](db::DatabaseSession &session) {
        return repository::Experiments{session}.getLazyActive();
    });

    return SingleExperimentInfo{repoExp};
}

std::vector<ExperimentInfo> ExperimentCommands::getAllReady() {
    auto repoExperiments = db.execute([](db::DatabaseSession &session) {
        return repository::Experiments{session}.getAllReady();
    });

    std::sort(repoExperiments.begin(), repoExperiments.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name < rhs.name;
    });
    return std::vector<ExperimentInfo>(repoExperiments.begin(), repoExperiments.end());
}

std::vector<ExperimentInfo> ExperimentCommands::getAllFinished() {
    auto repoExperiments = db.execute([](db::DatabaseSession &session) {
        return repository::Experiments{session}.getAllFinished();
    });

    std::sort(repoExperiments.begin(), repoExperiments.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.startDate > rhs.startDate;
    });
    return std::vector<ExperimentInfo>(repoExperiments.begin(), repoExperiments.end());
}

Experiment ExperimentCommands::get(std::int32_t ID) {
    auto repoExpr = db.execute([&](db::DatabaseSession &session) {
        if (auto exp = repository::Experiments{session}.get(ID)) {
            return exp.value();
        } else {
            throw io::InvalidInput{"experiment with given id doesn't exist"};
        }
    });

    return Experiment{repoExpr};
}

CurrentExperimentResponse ExperimentCommands::getCurrent() {
    boost::optional<repository::Experiment> repoExperiment =
        db.execute([&](db::DatabaseSession &session) {
            auto repo = repository::Experiments{session};
            return repo.getActive();
        });

    auto result = CurrentExperimentResponse{};
    if (repoExperiment) {
        result.experiment = Experiment{repoExperiment.value()};
    }
    return result;
}

repository::Experiments::LazyExperiment::Survey ExperimentCommands::toRepoSurvey(
    const QuestionsIdsList &qList) {
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