#ifndef SERVER_COMMAND__FINISH_EXPERIMENT__H
#define SERVER_COMMAND__FINISH_EXPERIMENT__H

#include <db/Database.h>

#include "Command.h"

namespace server {
namespace command {

class FinishExperiment : public Command {
public:
    FinishExperiment(db::Database &db);

    void operator()();

private:
    db::Database &db;
};
}
}

#endif