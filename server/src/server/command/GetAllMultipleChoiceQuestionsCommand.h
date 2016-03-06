#ifndef SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/MultipleChoiceQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllMultipleChoiceQuestionsCommand {
public:
    GetAllMultipleChoiceQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllMultipleChoiceQuestionsCommand);

    std::vector<io::output::MultipleChoiceQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif