#include <algorithm>

#include <repository/Exhibits.h>

#include <server/io/InvalidInput.h>
#include <server/utils/CmpUTF8.h>

#include "ExperimentCommands.h"

namespace server {
namespace command {

ExperimentCommands::ExperimentCommands(db::Database &db) : db(db) {
}

void ExperimentCommands::create(const CreateExperimentRequest &input) {
    auto lazyExperiment = toRepo(input);
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        repo.insert(&lazyExperiment);
    });
}

void ExperimentCommands::update(std::int32_t ID, const CreateExperimentRequest &input) {
    auto lazyExperiment = toRepo(input);
    lazyExperiment.ID = ID;
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        repo.update(lazyExperiment);
    });
}

void ExperimentCommands::clone(const CloneExperimentRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        repo.clone(input.ID, input.name);
    });
}

void ExperimentCommands::finish() {
    db.execute([](db::DatabaseSession &session) {
        repository::Experiments{session}.finishActive();
        repository::Exhibits{session}.refresh();
    });
}

void ExperimentCommands::start(std::int32_t ID) {
    db.execute([&](db::DatabaseSession &session) { repository::Experiments{session}.start(ID); });
}

void ExperimentCommands::remove(std::int32_t ID) {
    db.execute([&](db::DatabaseSession &session) { repository::Experiments{session}.remove(ID); });
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
        return utils::cmpUTF8(lhs.name, rhs.name);
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
    auto repoExpr = db.execute(
        [&](db::DatabaseSession &session) { return repository::Experiments{session}.get(ID); });

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

repository::Experiments::LazyExperiment ExperimentCommands::toRepo(
    const CreateExperimentRequest &request) const {
    auto res = repository::Experiments::LazyExperiment{};
    res.name = request.name;
    res.actions = request.actions;
    res.breakActions = request.breakActions;
    res.surveyBefore = toRepoSurvey(request.surveyBefore);
    res.surveyAfter = toRepoSurvey(request.surveyAfter);
    return res;
}

repository::Experiments::LazyExperiment::Survey ExperimentCommands::toRepoSurvey(
    const QuestionsIdsList &qList) const {
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