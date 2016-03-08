#include <utils/fp_algorithm.h>

#include <repository/SortQuestions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "CreateSortQuestionCommand.h"

namespace server {
namespace command {

CreateSortQuestionCommand::CreateSortQuestionCommand(db::Database &db) : db(db) {
}

io::output::SortQuestion CreateSortQuestionCommand::operator()(
    const io::input::CreateSortQuestionRequest &input) {
    auto dbQuestion = db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        auto question = repository::SortQuestion{};

        question.question = input.question;
        if (input.name) {
            question.name = input.name.value();
        } else {
            question.name = question.question;
        }
        ::utils::transform(input.options, question.options, [](auto &text) {
            auto opt = repository::SortQuestion::Option{};
            opt.text = text;
            return opt;
        });

        auto repo = repository::SortQuestions{session};
        repo.insert(&question);
        return question;
    });

    return io::output::SortQuestion{dbQuestion};
}

void CreateSortQuestionCommand::validateInput(
    db::DatabaseSession &session, const io::input::CreateSortQuestionRequest &input) const {
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