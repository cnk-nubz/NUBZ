#ifndef SERVER_COMMAND__GET_ACTIVE_EXPERIMENT__H
#define SERVER_COMMAND__GET_ACTIVE_EXPERIMENT__H

#include <db/Database.h>

#include <server/io/output/SingleExperimentInfo.h>

#include "Command.h"

namespace server {
namespace command {

class GetActiveExperiment : public Command {
public:
    GetActiveExperiment(db::Database &db);

    io::output::SingleExperimentInfo operator()();

private:
    db::Database &db;
};
}
}

#endif