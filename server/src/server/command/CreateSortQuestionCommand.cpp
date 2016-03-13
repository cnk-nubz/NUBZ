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
        validateInput(input);

        auto question = repository::SortQuestion{};

        question.question = input.question;
        question.name = input.name.value_or(question.question);
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
    const io::input::CreateSortQuestionRequest &input) const {
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