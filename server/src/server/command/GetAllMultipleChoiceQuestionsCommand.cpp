#include <repository/MultipleChoiceQuestions.h>

#include <server/io/utils.h>

#include "GetAllMultipleChoiceQuestionsCommand.h"

namespace server {
namespace command {

GetAllMultipleChoiceQuestionsCommand::GetAllMultipleChoiceQuestionsCommand(db::Database &db)
    : db(db) {
}

std::vector<io::output::MultipleChoiceQuestion> GetAllMultipleChoiceQuestionsCommand::operator()() {
    auto questions = std::vector<io::output::MultipleChoiceQuestion>{};
    auto repoQuestions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::MultipleChoiceQuestions{session};
        return repo.getAll();
    });

    auto result =
        std::vector<io::output::MultipleChoiceQuestion>(repoQuestions.begin(), repoQuestions.end());
    std::sort(result.begin(), result.end());
    return result;
}
}
}