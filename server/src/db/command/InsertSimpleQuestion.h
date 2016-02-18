#ifndef DB_CMD__INSERT_SIMPLE_QUESTION__H
#define DB_CMD__INSERT_SIMPLE_QUESTION__H

#include <db/DatabaseSession.h>
#include <db/struct/SimpleQuestion.h>

namespace db {
namespace cmd {

class InsertSimpleQuestion {
public:
    InsertSimpleQuestion(const SimpleQuestion &sQuestion);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const SimpleQuestion sQuestion;
    std::int32_t sQuestionId;

    std::string createInsert() const;
};
}
}

#endif