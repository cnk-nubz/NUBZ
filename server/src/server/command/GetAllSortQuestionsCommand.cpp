#include <utils/fp_algorithm.h>

#include <db/command/GetSortQuestions.h>
#include <db/command/GetSortQuestionOptions.h>

#include <server/utils/io_translation.h>

#include "GetAllSortQuestionsCommand.h"

namespace server {
namespace command {

GetAllSortQuestionsCommand::GetAllSortQuestionsCommand(db::Database &db) : db(db) {
}

std::vector<io::SortQuestion> GetAllSortQuestionsCommand::operator()() {
    std::vector<io::SortQuestion> questions;
    db.execute([&](db::DatabaseSession &session) {
        db::cmd::GetSortQuestions getQuestions;
        for (const auto &question : getQuestions(session)) {
            questions.push_back(utils::toIO(question, getOptions(session, question.ID)));
        }
    });

    std::sort(questions.begin(), questions.end());
    return questions;
}

std::vector<db::SortQuestionOption> GetAllSortQuestionsCommand::getOptions(
    db::DatabaseSession &session, std::int32_t questionId) const {
    return db::cmd::GetSortQuestionOptions{questionId}(session);
}
}
}