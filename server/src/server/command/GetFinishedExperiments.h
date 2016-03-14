#ifndef SERVER_COMMAND__GET_FINISHED_EXPERIMENTS__H
#define SERVER_COMMAND__GET_FINISHED_EXPERIMENTS__H

#include <db/Database.h>

#include <server/io/output/ExperimentInfo.h>

#include "Command.h"

namespace server {
namespace command {

class GetFinishedExperiments : public Command {
public:
    GetFinishedExperiments(db::Database &db);

    std::vector<io::output::ExperimentInfo> operator()();

private:
    db::Database &db;
};
}
}

#endif