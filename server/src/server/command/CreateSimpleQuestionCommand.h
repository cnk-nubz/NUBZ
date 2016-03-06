#ifndef SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateSimpleQuestionRequest.h>
#include <server/io/output/SimpleQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class CreateSimpleQuestionCommand {
public:
    CreateSimpleQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateSimpleQuestionCommand);

    io::output::SimpleQuestion operator()(const io::input::CreateSimpleQuestionRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateSimpleQuestionRequest &input) const;

    db::Database &db;
};
}
}

#endif