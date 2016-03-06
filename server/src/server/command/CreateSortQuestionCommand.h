#ifndef SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateSortQuestionRequest.h>
#include <server/io/output/SortQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class CreateSortQuestionCommand {
public:
    CreateSortQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateSortQuestionCommand);

    io::output::SortQuestion operator()(const io::input::CreateSortQuestionRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateSortQuestionRequest &input) const;

    db::Database &db;
};
}
}

#endif