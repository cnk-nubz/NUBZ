#ifndef DB_CMD__INSERT_MULTIPLE_CHOICE_QUESTION_OPTION__H
#define DB_CMD__INSERT_MULTIPLE_CHOICE_QUESTION_OPTION__H

#include <db/DatabaseSession.h>
#include <db/struct/MultipleChoiceQuestionOption.h>

namespace db {
namespace cmd {

class InsertMultipleChoiceQuestionOption {
public:
    InsertMultipleChoiceQuestionOption(const MultipleChoiceQuestionOption &questionOption);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const MultipleChoiceQuestionOption questionOption;
    std::int32_t questionOptionId;

    std::string createInsert() const;
};
}
}

#endif