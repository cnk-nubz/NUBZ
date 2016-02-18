#include <utils/fp_algorithm.h>

#include <db/command/GetMultipleChoiceQuestions.h>
#include <db/command/GetMultipleChoiceQuestionOptions.h>

#include <server/utils/io_translation.h>

#include "GetAllMultipleChoiceQuestionsCommand.h"

namespace server {
namespace command {

GetAllMultipleChoiceQuestionsCommand::GetAllMultipleChoiceQuestionsCommand(db::Database &db)
    : db(db) {
}

std::vector<io::MultipleChoiceQuestion> GetAllMultipleChoiceQuestionsCommand::operator()() {
    std::vector<io::MultipleChoiceQuestion> questions;
    db.execute([&](db::DatabaseSession &session) {
        db::cmd::GetMultipleChoiceQuestions getQuestions;
        for (const auto &question : getQuestions(session)) {
            questions.push_back(utils::toIO(question, getOptions(session, question.ID)));
        }
    });

    std::sort(questions.begin(),
              questions.end(),
              [](const auto &lhs, const auto &rhs) { return lhs.name < rhs.name; });
    return questions;
}

std::vector<db::MultipleChoiceQuestionOption> GetAllMultipleChoiceQuestionsCommand::getOptions(
    db::DatabaseSession &session, std::int32_t questionId) const {
    return db::cmd::GetMultipleChoiceQuestionOptions{questionId}(session);
}
}
}