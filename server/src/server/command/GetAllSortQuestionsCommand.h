#ifndef SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_SORT_QUESTIONS_COMMAND__H

#include <db/Database.h>
#include <db/struct/SortQuestionOption.h>

#include <server/io/SortQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllSortQuestionsCommand {
public:
    GetAllSortQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllSortQuestionsCommand);

    std::vector<io::SortQuestion> operator()();

private:
    db::Database &db;

    std::vector<db::SortQuestionOption> getOptions(db::DatabaseSession &session,
                                                   std::int32_t questionId) const;
};
}
}

#endif