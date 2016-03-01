#ifndef SERVER_COMMAND__GET_ALL_ACTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_ACTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/Action.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllActionsCommand {
public:
    GetAllActionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllActionsCommand);

    std::vector<io::Action> operator()();

private:
    db::Database &db;
};
}
}

#endif