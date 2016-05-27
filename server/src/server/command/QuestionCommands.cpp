#include <algorithm>

#include <utils/fp_algorithm.h>

#include <repository/MultipleChoiceQuestions.h>
#include <repository/SimpleQuestions.h>
#include <repository/SortQuestions.h>

#include <server/io/InvalidInput.h>

#include "QuestionCommands.h"

namespace server {
namespace command {

QuestionCommands::QuestionCommands(db::Database &db) : db(db) {
}

MultipleChoiceQuestion QuestionCommands::createMultipleChoice(
    const CreateMultipleChoiceQuestionRequest &input) {
    auto question = repository::MultipleChoiceQuestion{};
    question.name = input.name;
    question.question = input.question;
    question.singleAnswer = input.singleAnswer;
    ::utils::transform(input.options, question.options, [](auto &text) {
        auto opt = repository::MultipleChoiceQuestion::Option{};
        opt.text = text;
        return opt;
    });

    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::MultipleChoiceQuestions{session};
        repo.insert(&question);
    });

    return MultipleChoiceQuestion{question};
}

SimpleQuestion QuestionCommands::createSimple(const CreateSimpleQuestionRequest &input) {
    auto question = repository::SimpleQuestion{};
    question.name = input.name;
    question.question = input.question;
    question.numberAnswer = input.answerType == SimpleQuestion::AnswerType::Number;

    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::SimpleQuestions{session};
        repo.insert(&question);
        return question;
    });
    return SimpleQuestion{question};
}

SortQuestion QuestionCommands::createSort(const CreateSortQuestionRequest &input) {
    auto question = repository::SortQuestion{};
    question.name = input.name;
    question.question = input.question;
    ::utils::transform(input.options, question.options, [](auto &text) {
        auto opt = repository::SortQuestion::Option{};
        opt.text = text;
        return opt;
    });

    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::SortQuestions{session};
        repo.insert(&question);
    });

    return SortQuestion{question};
}

QuestionsList QuestionCommands::getAll() {
    auto all = std::vector<std::pair<Question, io::QuestionType>>{};
    auto qList = QuestionsList{};
    db.execute([&](db::DatabaseSession &session) {
        for (const auto &repoQ : repository::SimpleQuestions{session}.getAll()) {
            auto ioQ = SimpleQuestion{repoQ};
            qList.simpleQuestions.push_back(ioQ);
            all.emplace_back(ioQ, io::QuestionType::Simple);
        }
        for (const auto &repoQ : repository::MultipleChoiceQuestions{session}.getAll()) {
            auto ioQ = MultipleChoiceQuestion{repoQ};
            qList.multipleChoiceQuestions.push_back(ioQ);
            all.emplace_back(ioQ, io::QuestionType::MultipleChoice);
        }
        for (const auto &repoQ : repository::SortQuestions{session}.getAll()) {
            auto ioQ = SortQuestion{repoQ};
            qList.sortQuestions.push_back(ioQ);
            all.emplace_back(ioQ, io::QuestionType::Sort);
        }
    });

    std::sort(qList.simpleQuestions.begin(), qList.simpleQuestions.end());
    std::sort(qList.multipleChoiceQuestions.begin(), qList.multipleChoiceQuestions.end());
    std::sort(qList.sortQuestions.begin(), qList.sortQuestions.end());
    std::sort(all.begin(), all.end());
    for (const auto &q : all) {
        qList.questionsOrder.push_back(q.second);
    }
    return qList;
}

std::vector<SimpleQuestion> QuestionCommands::getAllSimple() {
    auto repoQuestions = db.execute(
        [](db::DatabaseSession &session) { return repository::SimpleQuestions{session}.getAll(); });

    auto questions = std::vector<SimpleQuestion>(repoQuestions.begin(), repoQuestions.end());
    std::sort(questions.begin(), questions.end());
    return questions;
}

std::vector<MultipleChoiceQuestion> QuestionCommands::getAllMultipleChoice() {
    auto repoQuestions = db.execute([](db::DatabaseSession &session) {
        return repository::MultipleChoiceQuestions{session}.getAll();
    });

    auto result = std::vector<MultipleChoiceQuestion>(repoQuestions.begin(), repoQuestions.end());
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<SortQuestion> QuestionCommands::getAllSort() {
    auto repoQuestions = db.execute(
        [](db::DatabaseSession &session) { return repository::SortQuestions{session}.getAll(); });

    auto result = std::vector<SortQuestion>(repoQuestions.begin(), repoQuestions.end());
    std::sort(result.begin(), result.end());
    return result;
}

void QuestionCommands::removeSimple(std::int32_t ID) {
    db.execute(
        [&](db::DatabaseSession &session) { repository::SimpleQuestions{session}.remove(ID); });
}

void QuestionCommands::removeMultipleChoice(std::int32_t ID) {
    db.execute([&](db::DatabaseSession &session) {
        repository::MultipleChoiceQuestions{session}.remove(ID);
    });
}

void QuestionCommands::removeSort(std::int32_t ID) {
    db.execute(
        [&](db::DatabaseSession &session) { repository::SortQuestions{session}.remove(ID); });
}
}
}
