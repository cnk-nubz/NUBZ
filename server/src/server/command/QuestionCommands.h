#ifndef SERVER_COMMAND__QUESTION_COMMANDS__H
#define SERVER_COMMAND__QUESTION_COMMANDS__H

#include <vector>

#include <db/Database.h>

#include <server/io/input/CreateMultipleChoiceQuestionRequest.h>
#include <server/io/input/CreateSimpleQuestionRequest.h>
#include <server/io/input/CreateSortQuestionRequest.h>
#include <server/io/output/MultipleChoiceQuestion.h>
#include <server/io/output/MultipleChoiceQuestion.h>
#include <server/io/output/QuestionsList.h>
#include <server/io/output/SimpleQuestion.h>
#include <server/io/output/SimpleQuestion.h>
#include <server/io/output/SortQuestion.h>
#include <server/io/output/SortQuestion.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class QuestionCommands : public Command {
public:
    QuestionCommands(db::Database &db);

    MultipleChoiceQuestion createMultipleChoice(const CreateMultipleChoiceQuestionRequest &input);
    SimpleQuestion createSimple(const CreateSimpleQuestionRequest &input);
    SortQuestion createSort(const CreateSortQuestionRequest &input);

    QuestionsList getAll();
    std::vector<SimpleQuestion> getAllSimple();
    std::vector<MultipleChoiceQuestion> getAllMultipleChoice();
    std::vector<SortQuestion> getAllSort();

    void removeSimple(std::int32_t ID);
    void removeMultipleChoice(std::int32_t ID);
    void removeSort(std::int32_t ID);

private:
    db::Database &db;
};
}
}

#endif