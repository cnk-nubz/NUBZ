#ifndef SERVER_COMMAND__GET_ALL_SIMPLE_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_SIMPLE_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/SimpleQuestion.h>

#include "Command.h"

namespace server {
namespace command {

class GetAllSimpleQuestionsCommand : public Command {
public:
    GetAllSimpleQuestionsCommand(db::Database &db);

    std::vector<io::output::SimpleQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif