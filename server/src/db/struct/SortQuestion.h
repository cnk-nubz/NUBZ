#ifndef DB_STRUCT__SORT_QUESTION__H
#define DB_STRUCT__SORT_QUESTION__H

#include <cstdint>
#include <string>

namespace db {
struct SortQuestion {
    std::int32_t ID;
    std::string name;
    std::string question;
};
}

#endif