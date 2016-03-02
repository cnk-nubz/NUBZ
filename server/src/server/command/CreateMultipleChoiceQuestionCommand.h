#ifndef SERVER_COMMAND__CREATE_MULTIPLE_CHOICE_QUESTION_COMMAND__H
#define SERVER_COMMAND__CREATE_MULTIPLE_CHOICE_QUESTION_COMMAND__H

#include <db/Database.h>
#include <db/struct/MultipleChoiceQuestionOption.h>

#include <server/io/MultipleChoiceQuestion.h>
#include <server/io/input/CreateMultipleChoiceQuestionRequest.h>

#include "commons.h"

namespace server {
namespace command {

class CreateMultipleChoiceQuestionCommand {
public:
    CreateMultipleChoiceQuestionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateMultipleChoiceQuestionCommand);

    io::MultipleChoiceQuestion operator()(
        const io::input::CreateMultipleChoiceQuestionRequest &input);

private:
    db::Database &db;

    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateMultipleChoiceQuestionRequest &input) const;
    std::vector<db::MultipleChoiceQuestionOption> createOptions(
        db::DatabaseSession &session, const std::vector<std::string> &optionsText,
        std::int32_t questionId) const;
};
}
}

#endif