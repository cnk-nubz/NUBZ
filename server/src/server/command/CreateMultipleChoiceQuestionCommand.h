#ifndef SERVER_COMMAND__CREATE_MULTIPLE_CHOICE_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_MULTIPLE_CHOICE_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateMultipleChoiceQuestionRequest.h>
#include <server/io/output/MultipleChoiceQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class CreateMultipleChoiceQuestionCommand {
public:
    CreateMultipleChoiceQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateMultipleChoiceQuestionCommand);

    io::output::MultipleChoiceQuestion operator()(
        const io::input::CreateMultipleChoiceQuestionRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateMultipleChoiceQuestionRequest &input) const;

    db::Database &db;
};
}
}

#endif