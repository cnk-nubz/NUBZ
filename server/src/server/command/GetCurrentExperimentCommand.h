#ifndef SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H
#define SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H

#include <db/Database.h>

#include <server/io/output/CurrentExperimentResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetCurrentExperimentCommand {
public:
    GetCurrentExperimentCommand(db::Database &db);
    SRV_CMD_CP_MV(GetCurrentExperimentCommand);

    io::output::CurrentExperimentResponse operator()();

private:
    db::Database &db;
};
}
}

#endif