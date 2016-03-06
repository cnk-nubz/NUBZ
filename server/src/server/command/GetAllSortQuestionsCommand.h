#ifndef SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/SortQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllSortQuestionsCommand {
public:
    GetAllSortQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllSortQuestionsCommand);

    std::vector<io::output::SortQuestion> operator()();

private:
    db::Database &db;
};
}
}

#endif