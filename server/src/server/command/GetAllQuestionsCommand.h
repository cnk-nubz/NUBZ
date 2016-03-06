#ifndef SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/QuestionsList.h>

#include "GetAllMultipleChoiceQuestionsCommand.h"
#include "GetAllSimpleQuestionsCommand.h"
#include "GetAllSortQuestionsCommand.h"
#include "commons.h"

namespace server {
namespace command {

class GetAllQuestionsCommand {
public:
    GetAllQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllQuestionsCommand);

    io::QuestionsList operator()();

private:
    void generateOrder(io::QuestionsList &list) const;

    GetAllSimpleQuestionsCommand getSimple;
    GetAllMultipleChoiceQuestionsCommand getMultiple;
    GetAllSortQuestionsCommand getSortQ;
};
}
}

#endif