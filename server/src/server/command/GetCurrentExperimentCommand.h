#ifndef SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H
#define SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H

#include <db/Database.h>
#include <db/struct/Experiment.h>

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

    // temporary data, correct only inside operator()
    io::output::Experiment currentExperiment;

    void fillExperimentData(const db::Experiment &experiment, db::DatabaseSession &session);
};
}
}

#endif