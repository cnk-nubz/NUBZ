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
        validateInput(session, input);

        auto question = repository::MultipleChoiceQuestion{};
        question.question = input.question;
        question.singleAnswer = input.singleAnswer;
        if (input.name) {
            question.name = input.name.value();
        } else {
            question.name = question.question;
        }
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
    db::DatabaseSession &session,
    const io::input::CreateMultipleChoiceQuestionRequest &input) const {
    auto checker = utils::InputChecker{session};
    if (!checker.checkText(input.question)) {
        throw io::InvalidInput("incorrect question");
    }
    if (input.name && !checker.checkText(input.name.value())) {
        throw io::InvalidInput("incorrect name");
    }
    if (input.options.size() < 2) {
        throw io::InvalidInput("options list should contain at least 2 options");
    }
    if (!::utils::all_of(input.options, &decltype(checker)::checkText)) {
        throw io::InvalidInput("incorrect option");
    }
}
}
}