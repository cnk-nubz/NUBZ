#include "GetAllQuestionsCommand.h"

namespace server {
namespace command {

GetAllQuestionsCommand::GetAllQuestionsCommand(db::Database &db)
    : getSimple(db), getMultiple(db), getSortQ(db) {
}

io::output::QuestionsList GetAllQuestionsCommand::operator()() {
    auto qList = io::output::QuestionsList{};
    qList.simpleQuestions = getSimple();
    qList.multipleChoiceQuestions = getMultiple();
    qList.sortQuestions = getSortQ();
    generateOrder(qList);
    return qList;
}

void GetAllQuestionsCommand::generateOrder(io::output::QuestionsList &list) const {
    auto all = std::vector<std::pair<io::output::Question, io::QuestionType>>{};
    for (const auto &q : list.simpleQuestions) {
        all.emplace_back(q, io::QuestionType::Simple);
    }
    for (const auto &q : list.multipleChoiceQuestions) {
        all.emplace_back(q, io::QuestionType::MultipleChoice);
    }
    for (const auto &q : list.sortQuestions) {
        all.emplace_back(q, io::QuestionType::Sort);
    }
    std::sort(all.begin(), all.end());

    for (const auto &t : all) {
        list.questionsOrder.push_back(t.second);
    }
}
}
}