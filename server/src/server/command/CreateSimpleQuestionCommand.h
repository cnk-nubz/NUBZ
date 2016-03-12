#ifndef SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SIMPLE_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateSimpleQuestionRequest.h>
#include <server/io/output/SimpleQuestion.h>

#include "Command.h"

namespace server {
namespace command {

class CreateSimpleQuestionCommand : public Command {
public:
    CreateSimpleQuestionCommand(db::Database &db);

    io::output::SimpleQuestion operator()(const io::input::CreateSimpleQuestionRequest &input);

private:
    void validateInput(const io::input::CreateSimpleQuestionRequest &input) const;

    db::Database &db;
};
}
}

#endif