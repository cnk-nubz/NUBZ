#include <repository/MultiplechoiceQuestions.h>

#include <server/io/utils.h>

#include "GetAllMultipleChoiceQuestionsCommand.h"

namespace server {
namespace command {

GetAllMultipleChoiceQuestionsCommand::GetAllMultipleChoiceQuestionsCommand(db::Database &db)
    : db(db) {
}

std::vector<io::MultipleChoiceQuestion> GetAllMultipleChoiceQuestionsCommand::operator()() {
    auto questions = std::vector<io::MultipleChoiceQuestion>{};
    auto repoQuestions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::MultipleChoiceQuestions{session};
        return repo.getAll();
    });

    auto result = ::server::io::repoToIO<io::MultipleChoiceQuestion>(repoQuestions);
    std::sort(result.begin(), result.end());
    return result;
}
}
}