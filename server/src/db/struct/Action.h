#ifndef DB_STRUCT__ACTION__H
#define DB_STRUCT__ACTION__H

#include <cstdint>
#include <string>

namespace db {
    struct Action {
        std::int32_t ID;
        std::string text;
        bool duringBreak;
        bool inCurrentExperiment;
    };
}

#endif