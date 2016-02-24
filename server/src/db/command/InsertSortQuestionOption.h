#ifndef DB_CMD__INSERT_SORT_QUESTION_OPTION__H
#define DB_CMD__INSERT_SORT_QUESTION_OPTION__H

#include <db/DatabaseSession.h>
#include <db/struct/SortQuestionOption.h>

namespace db {
namespace cmd {

class InsertSortQuestionOption {
public:
    InsertSortQuestionOption(const SortQuestionOption &questionOption);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const SortQuestionOption questionOption;
    std::int32_t questionOptionId;

    std::string createInsert() const;
};
}
}

#endif