#ifndef SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H
#define SERVER_COMMAND__GET_CURRENT_EXPERIMENT_COMMAND__H

#include <db/Database.h>

#include <server/io/output/CurrentExperimentResponse.h>

#include "Command.h"

namespace server {
namespace command {

class GetCurrentExperimentCommand : public Command {
public:
    GetCurrentExperimentCommand(db::Database &db);

    io::output::CurrentExperimentResponse operator()();

private:
    db::Database &db;
};
}
}

#endif