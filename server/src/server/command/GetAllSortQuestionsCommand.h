#ifndef SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/SortQuestion.h>

#include "Command.h"

namespace server {
namespace command {

class GetAllSortQuestionsCommand : public Command {
public:
    GetAllSortQuestionsCommand(db::Database &db);

    std::vector<io::output::SortQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif