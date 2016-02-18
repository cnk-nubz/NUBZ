#ifndef DB_CMD__INSERT_MULTIPLE_CHOICE_QUESTION__H
#define DB_CMD__INSERT_MULTIPLE_CHOICE_QUESTION__H

#include <db/DatabaseSession.h>
#include <db/struct/MultipleChoiceQuestion.h>

namespace db {
namespace cmd {

class InsertMultipleChoiceQuestion {
public:
    InsertMultipleChoiceQuestion(const MultipleChoiceQuestion &question);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const MultipleChoiceQuestion question;
    std::int32_t questionId;

    std::string createInsert() const;
};
}
}

#endif