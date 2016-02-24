#include <db/command/InsertSortQuestion.h>
#include <db/command/InsertSortQuestionOption.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateSortQuestionCommand.h"

namespace server {
namespace command {

CreateSortQuestionCommand::CreateSortQuestionCommand(db::Database &db) : db(db) {
}

io::SortQuestion CreateSortQuestionCommand::operator()(
    const io::input::CreateSortQuestionRequest &input) {
    io::SortQuestion ioQuestion;
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::SortQuestion question;
        question.name = input.question;
        question.question = input.question;
        if (input.name) {
            question.name = input.name.value();
        }
        question.ID = db::cmd::InsertSortQuestion{question}(session);

        ioQuestion = utils::toIO(question, createOptions(session, input.options, question.ID));
    });

    return ioQuestion;
}

void CreateSortQuestionCommand::validateInput(
    db::DatabaseSession &session, const io::input::CreateSortQuestionRequest &input) const {
    utils::InputChecker checker(session);
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

std::vector<db::SortQuestionOption> CreateSortQuestionCommand::createOptions(
    db::DatabaseSession &session, const std::vector<std::string> &optionsText,
    std::int32_t questionId) const {
    std::vector<db::SortQuestionOption> options;
    db::SortQuestionOption option;
    option.questionId = questionId;
    for (const auto &text : optionsText) {
        option.text = text;
        option.ID = db::cmd::InsertSortQuestionOption{option}(session);
        options.push_back(option);
    }
    return options;
}
}
}