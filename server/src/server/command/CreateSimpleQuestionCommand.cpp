#include <db/command/InsertSimpleQuestion.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateSimpleQuestionCommand.h"

namespace server {
namespace command {

CreateSimpleQuestionCommand::CreateSimpleQuestionCommand(db::Database &db) : db(db) {
}

io::SimpleQuestion CreateSimpleQuestionCommand::operator()(
    const io::input::CreateSimpleQuestionRequest &input) {
    io::SimpleQuestion sQuestion;
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::SimpleQuestion dbSQuestion;
        dbSQuestion.name = input.question;
        dbSQuestion.question = input.question;
        if (input.name) {
            dbSQuestion.name = input.name.value();
        }
        dbSQuestion.numberAnswer = input.answerType == io::SimpleQuestion::AnswerType::Number;
        dbSQuestion.ID = db::cmd::InsertSimpleQuestion{dbSQuestion}(session);
        sQuestion = utils::toIO(dbSQuestion);
    });
    return sQuestion;
}

void CreateSimpleQuestionCommand::validateInput(
    db::DatabaseSession &session, const io::input::CreateSimpleQuestionRequest &input) const {
    utils::InputChecker checker(session);
    if (!checker.checkText(input.question)) {
        throw io::InvalidInput("incorrect question");
    }
    if (input.name && !checker.checkText(input.name.value())) {
        throw io::InvalidInput("incorrect name");
    }
}
}
}