#include <utils/fp_algorithm.h>

#include <repository/SimpleQuestions.h>

#include <server/utils/io_translation.h>

#include "GetAllSimpleQuestionsCommand.h"

namespace server {
namespace command {

GetAllSimpleQuestionsCommand::GetAllSimpleQuestionsCommand(db::Database &db) : db(db) {
}

std::vector<io::SimpleQuestion> GetAllSimpleQuestionsCommand::operator()() {
    auto dbQuestions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::SimpleQuestions{session};
        return repo.getAll();
    });

    auto questions = utils::toIO<io::SimpleQuestion>(dbQuestions);
    std::sort(questions.begin(), questions.end());
    return questions;
}
}
}