#ifndef SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/MultipleChoiceQuestion.h>

#include "Command.h"

namespace server {
namespace command {

class GetAllMultipleChoiceQuestionsCommand : public Command {
public:
    GetAllMultipleChoiceQuestionsCommand(db::Database &db);

    std::vector<io::output::MultipleChoiceQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif