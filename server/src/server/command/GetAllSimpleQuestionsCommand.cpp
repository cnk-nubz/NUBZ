#include <utils/fp_algorithm.h>

#include <repository/SimpleQuestions.h>

#include <server/io/utils.h>

#include "GetAllSimpleQuestionsCommand.h"

namespace server {
namespace command {

GetAllSimpleQuestionsCommand::GetAllSimpleQuestionsCommand(db::Database &db) : db(db) {
}

std::vector<io::output::SimpleQuestion> GetAllSimpleQuestionsCommand::operator()() {
    auto repoQuestions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::SimpleQuestions{session};
        return repo.getAll();
    });

    auto questions =
        std::vector<io::output::SimpleQuestion>(repoQuestions.begin(), repoQuestions.end());
    std::sort(questions.begin(), questions.end());
    return questions;
}
}
}