#ifndef SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/SimpleQuestion.h>
#include <server/io/input/CreateSimpleQuestionRequest.h>

#include "commons.h"

namespace server {
namespace command {

class CreateSimpleQuestionCommand {
public:
    CreateSimpleQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateSimpleQuestionCommand);

    io::SimpleQuestion operator()(const io::input::CreateSimpleQuestionRequest &input);

private:
    db::Database &db;

    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateSimpleQuestionRequest &input) const;
};
}
}

#endif