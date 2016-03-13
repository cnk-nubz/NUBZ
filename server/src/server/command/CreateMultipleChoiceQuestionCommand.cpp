#include <repository/MultipleChoiceQuestions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "CreateMultipleChoiceQuestionCommand.h"

namespace server {
namespace command {

CreateMultipleChoiceQuestionCommand::CreateMultipleChoiceQuestionCommand(db::Database &db)
    : db(db) {
}

io::output::MultipleChoiceQuestion CreateMultipleChoiceQuestionCommand::operator()(
    const io::input::CreateMultipleChoiceQuestionRequest &input) {
    auto dbQuestion = db.execute([&](db::DatabaseSession &session) {
        validateInput(input);

        auto question = repository::MultipleChoiceQuestion{};
        question.question = input.question;
        question.singleAnswer = input.singleAnswer;
        question.name = input.name.value_or(question.question);
        ::utils::transform(input.options, question.options, [](auto &text) {
            auto opt = repository::MultipleChoiceQuestion::Option{};
            opt.text = text;
            return opt;
        });

        auto repo = repository::MultipleChoiceQuestions{session};
        repo.insert(&question);
        return question;
    });

    return io::output::MultipleChoiceQuestion{dbQuestion};
}

void CreateMultipleChoiceQuestionCommand::validateInput(
    const io::input::CreateMultipleChoiceQuestionRequest &input) const {
    if (!utils::checkText(input.question)) {
        throw io::InvalidInput("incorrect question");
    }
    if (input.name && !utils::checkText(input.name.value())) {
        throw io::InvalidInput("incorrect name");
    }
    if (!::utils::all_of(input.options, utils::checkText)) {
        throw io::InvalidInput("incorrect option");
    }
}
}
}