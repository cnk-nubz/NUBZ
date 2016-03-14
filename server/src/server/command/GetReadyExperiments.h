#ifndef SERVER_COMMAND__GET_READY_EXPERIMENTS__H
#define SERVER_COMMAND__GET_READY_EXPERIMENTS__H

#include <db/Database.h>

#include <server/io/output/ExperimentInfo.h>

#include "Command.h"

namespace server {
namespace command {

class GetReadyExperiments : public Command {
public:
    GetReadyExperiments(db::Database &db);

    std::vector<io::output::ExperimentInfo> operator()();

private:
    db::Database &db;
};
}
}

#endif