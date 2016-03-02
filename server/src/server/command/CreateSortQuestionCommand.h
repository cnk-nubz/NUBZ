#ifndef SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H

#include <db/Database.h>
#include <db/struct/SortQuestionOption.h>

#include <server/io/SortQuestion.h>
#include <server/io/input/CreateSortQuestionRequest.h>

#include "commons.h"

namespace server {
namespace command {

class CreateSortQuestionCommand {
public:
    CreateSortQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateSortQuestionCommand);

    io::SortQuestion operator()(const io::input::CreateSortQuestionRequest &input);

private:
    db::Database &db;

    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateSortQuestionRequest &input) const;
    std::vector<db::SortQuestionOption> createOptions(db::DatabaseSession &session,
                                                      const std::vector<std::string> &optionsText,
                                                      std::int32_t questionId) const;
};
}
}

#endif