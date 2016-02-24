#ifndef SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/QuestionsList.h>

#include "commons.h"
#include "GetAllSimpleQuestionsCommand.h"
#include "GetAllMultipleChoiceQuestionsCommand.h"
#include "GetAllSortQuestionsCommand.h"

namespace server {
namespace command {

class GetAllQuestionsCommand {
public:
    GetAllQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllQuestionsCommand);

    io::QuestionsList operator()();

private:
    GetAllSimpleQuestionsCommand getSimple;
    GetAllMultipleChoiceQuestionsCommand getMultiple;
    GetAllSortQuestionsCommand getSortQ;

    void generateOrder(io::QuestionsList &list) const;
};
}
}

#endif