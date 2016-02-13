#ifndef DB_STRUCT__SIMPLE_QUESTION__H
#define DB_STRUCT__SIMPLE_QUESTION__H

#include <cstdint>
#include <string>

namespace db {
struct SimpleQuestion {
    std::int32_t ID;
    std::string name;
    std::string question;
    bool numberAnswer;
};
}

#endif