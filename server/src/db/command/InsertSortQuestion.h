#ifndef DB_CMD__INSERT_SORT_QUESTION__H
#define DB_CMD__INSERT_SORT_QUESTION__H

#include <db/DatabaseSession.h>
#include <db/struct/SortQuestion.h>

namespace db {
namespace cmd {

class InsertSortQuestion {
public:
    InsertSortQuestion(const SortQuestion &question);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const SortQuestion question;
    std::int32_t questionId;

    std::string createInsert() const;
};
}
}

#endif