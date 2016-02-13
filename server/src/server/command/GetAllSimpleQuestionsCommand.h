#ifndef SERVER_COMMAND__GET_ALL_SIMPLE_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_SIMPLE_QUESTIONS_COMMAND__H

#include <db/Database.h>
#include <db/struct/SimpleQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllSimpleQuestionsCommand {
public:
    GetAllSimpleQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllSimpleQuestionsCommand);

    std::vector<io::SimpleQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif