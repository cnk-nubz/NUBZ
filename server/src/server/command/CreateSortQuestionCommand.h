#ifndef SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_SORT_QUESTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateSortQuestionRequest.h>
#include <server/io/output/SortQuestion.h>

#include "Command.h"

namespace server {
namespace command {

class CreateSortQuestionCommand : public Command {
public:
    CreateSortQuestionCommand(db::Database &db);

    io::output::SortQuestion operator()(const io::input::CreateSortQuestionRequest &input);

private:
    void validateInput(const io::input::CreateSortQuestionRequest &input) const;

    db::Database &db;
};
}
}

#endif