#ifndef SERVER_COMMAND__START_EXPERIMENT__H
#define SERVER_COMMAND__START_EXPERIMENT__H

#include <db/Database.h>

#include "Command.h"

namespace server {
namespace command {

class StartExperiment : public Command {
public:
    StartExperiment(db::Database &db);

    void operator()(std::int32_t ID);

private:
    db::Database &db;
};
}
}

#endif