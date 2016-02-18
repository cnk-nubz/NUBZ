#ifndef SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_MULTIPLE_CHOICE_QUESTIONS_COMMAND__H

#include <db/Database.h>
#include <db/struct/MultipleChoiceQuestionOption.h>

#include <server/io/MultipleChoiceQuestion.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllMultipleChoiceQuestionsCommand {
public:
    GetAllMultipleChoiceQuestionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllMultipleChoiceQuestionsCommand);

    std::vector<io::MultipleChoiceQuestion> operator()();

private:
    db::Database &db;

    std::vector<db::MultipleChoiceQuestionOption> getOptions(db::DatabaseSession &session,
                                                             std::int32_t questionId) const;
};
}
}

#endif