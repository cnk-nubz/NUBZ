#ifndef SERVER_COMMAND__GET_EXPERIMENT_DATA_COMMAND__H
#define SERVER_COMMAND__GET_EXPERIMENT_DATA_COMMAND__H

#include <db/Database.h>

#include <server/io/output/ExperimentData.h>

#include "commons.h"

namespace server {
namespace command {

class GetExperimentDataCommand {
public:
    GetExperimentDataCommand(db::Database &db);
    SRV_CMD_CP_MV(GetExperimentDataCommand);

    io::output::ExperimentData operator()();

private:
    db::Database &db;
};
}
}

#endif