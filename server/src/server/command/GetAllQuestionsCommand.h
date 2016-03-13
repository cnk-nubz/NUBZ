#ifndef SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/QuestionsList.h>

#include "Command.h"
#include "GetAllMultipleChoiceQuestionsCommand.h"
#include "GetAllSimpleQuestionsCommand.h"
#include "GetAllSortQuestionsCommand.h"

namespace server {
namespace command {

class GetAllQuestionsCommand : public Command {
public:
    GetAllQuestionsCommand(db::Database &db);

    io::output::QuestionsList operator()();

private:
    void generateOrder(io::output::QuestionsList &list) const;

    GetAllSimpleQuestionsCommand getSimple;
    GetAllMultipleChoiceQuestionsCommand getMultiple;
    GetAllSortQuestionsCommand getSortQ;
};
}
}

#endif