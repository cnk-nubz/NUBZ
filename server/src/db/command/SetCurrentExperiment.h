#ifndef DB_CMD__SET_CURRENT_EXPERIMENT__H
#define DB_CMD__SET_CURRENT_EXPERIMENT__H

#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class SetCurrentExperiment {
public:
    SetCurrentExperiment(std::int32_t newId);

    void operator()(DatabaseSession &session);

private:
    std::int32_t newId;

    std::string createUpdate() const;
};
}
}

#endif