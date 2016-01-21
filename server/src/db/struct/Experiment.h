#ifndef DB_STRUCT__EXPERIMENT__H
#define DB_STRUCT__EXPERIMENT__H

#include <cstdint>
#include <string>
#include <vector>

namespace db {

struct Experiment {
    std::int32_t ID;
    std::string name;
    std::vector<std::int32_t> actions;
    std::vector<std::int32_t> breakActions;
};
}

#endif