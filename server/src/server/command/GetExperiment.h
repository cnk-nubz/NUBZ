#ifndef SERVER_COMMAND__GET_EXPERIMENT__H
#define SERVER_COMMAND__GET_EXPERIMENT__H

#include <db/Database.h>

#include <server/io/output/Experiment.h>

#include "Command.h"

namespace server {
namespace command {

class GetExperiment : public Command {
public:
    GetExperiment(db::Database &db);

    io::output::Experiment operator()(std::int32_t ID);

private:
    db::Database &db;
};
}
}

#endif