#include <db/command/InsertMultipleChoiceQuestion.h>
#include <db/command/InsertMultipleChoiceQuestionOption.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateMultipleChoiceQuestionCommand.h"

namespace server {
namespace command {

CreateMultipleChoiceQuestionCommand::CreateMultipleChoiceQuestionCommand(db::Database &db)
    : db(db) {
}

io::MultipleChoiceQuestion CreateMultipleChoiceQuestionCommand::operator()(
    const io::input::CreateMultipleChoiceQuestionRequest &input) {
    io::MultipleChoiceQuestion ioQuestion;
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::MultipleChoiceQuestion question;
        question.name = input.question;
        question.question = input.question;
        if (input.name) {
            question.name = input.name.value();
        }
        question.singleAnswer = input.singleAnswer;
        question.ID = db::cmd::InsertMultipleChoiceQuestion{question}(session);

        ioQuestion = utils::toIO(question, createOptions(session, input.options, question.ID));
    });

    return ioQuestion;
}

void CreateMultipleChoiceQuestionCommand::validateInput(
    db::DatabaseSession &session,
    const io::input::CreateMultipleChoiceQuestionRequest &input) const {
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

std::vector<db::MultipleChoiceQuestionOption> CreateMultipleChoiceQuestionCommand::createOptions(
    db::DatabaseSession &session, const std::vector<std::string> &optionsText,
    std::int32_t questionId) const {
    std::vector<db::MultipleChoiceQuestionOption> options;
    db::MultipleChoiceQuestionOption option;
    option.questionId = questionId;
    for (const auto &text : optionsText) {
        option.text = text;
        option.ID = db::cmd::InsertMultipleChoiceQuestionOption{option}(session);
        options.push_back(option);
    }
    return options;
}
}
}