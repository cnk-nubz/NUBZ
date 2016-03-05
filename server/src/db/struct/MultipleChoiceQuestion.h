#ifndef DB_STRUCT__MULTIPLE_CHOICE_QUESTION__H
#define DB_STRUCT__MULTIPLE_CHOICE_QUESTION__H

#include <cstdint>
#include <string>

namespace db {
struct MultipleChoiceQuestion {
    std::int32_t ID;
    std::string name;
    std::string question;
    bool singleAnswer;
};
}

#endif