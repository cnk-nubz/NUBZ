#include "GetAllQuestionsCommand.h"

namespace server {
namespace command {

GetAllQuestionsCommand::GetAllQuestionsCommand(db::Database &db)
    : getSimple(db), getMultiple(db), getSortQ(db) {
}

io::QuestionsList GetAllQuestionsCommand::operator()() {
    io::QuestionsList qList;
    qList.simpleQuestions = getSimple();
    qList.multipleChoiceQuestions = getMultiple();
    qList.sortQuestions = getSortQ();
    generateOrder(qList);
    return qList;
}

void GetAllQuestionsCommand::generateOrder(io::QuestionsList &list) const {
    std::vector<std::pair<io::Question, io::QuestionsList::QuestionType>> all;
    for (const auto &q : list.simpleQuestions) {
        all.push_back({q, io::QuestionsList::QuestionType::Simple});
    }
    for (const auto &q : list.multipleChoiceQuestions) {
        all.push_back({q, io::QuestionsList::QuestionType::MultipleChoice});
    }
    for (const auto &q : list.sortQuestions) {
        all.push_back({q, io::QuestionsList::QuestionType::Sort});
    }
    std::sort(all.begin(), all.end());

    for (const auto &t : all) {
        list.questionsOrder.push_back(t.second);
    }
}
}
}