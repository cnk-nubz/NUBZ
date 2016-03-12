#include <repository/SimpleQuestions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "CreateSimpleQuestionCommand.h"

namespace server {
namespace command {

CreateSimpleQuestionCommand::CreateSimpleQuestionCommand(db::Database &db) : db(db) {
}

io::output::SimpleQuestion CreateSimpleQuestionCommand::operator()(
    const io::input::CreateSimpleQuestionRequest &input) {
    auto dbQuestion = db.execute([&](db::DatabaseSession &session) {
        validateInput(input);

        auto question = repository::SimpleQuestion{};
        question.question = input.question;
        question.numberAnswer = input.answerType == io::output::SimpleQuestion::AnswerType::Number;
        question.name = input.name.value_or(question.question);

        auto repo = repository::SimpleQuestions{session};
        repo.insert(&question);
        return question;
    });
    return io::output::SimpleQuestion{dbQuestion};
}

void CreateSimpleQuestionCommand::validateInput(
    const io::input::CreateSimpleQuestionRequest &input) const {
    if (!utils::checkText(input.question)) {
        throw io::InvalidInput("incorrect question");
    }
    if (input.name && !utils::checkText(input.name.value())) {
        throw io::InvalidInput("incorrect name");
    }
}
}
}