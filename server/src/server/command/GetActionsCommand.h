#ifndef SERVER_COMMAND__GET_ACTIONS_COMMAND__H
#define SERVER_COMMAND__GET_ACTIONS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/ActionsResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetActionsCommand {
public:
    GetActionsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetActionsCommand);

    io::output::ActionsResponse operator()();

private:
    db::Database &db;
};
}
}

#endif