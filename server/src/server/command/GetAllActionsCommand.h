#ifndef SERVER_COMMAND__GET_ALL_ACTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_ACTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/Action.h>

#include "Command.h"

namespace server {
namespace command {

class GetAllActionsCommand : public Command {
public:
    GetAllActionsCommand(db::Database &db);

    std::vector<io::output::Action> operator()();

private:
    db::Database &db;
};
}
}

#endif