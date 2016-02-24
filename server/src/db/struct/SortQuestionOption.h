#ifndef DB_STRUCT__SORT_QUESTION_OPTION__H
#define DB_STRUCT__SORT_QUESTION_OPTION__H

#include <cstdint>
#include <string>

namespace db {
struct SortQuestionOption {
    std::int32_t ID;
    std::int32_t questionId;
    std::string text;
};
}

#endif