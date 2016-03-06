#ifndef SERVER_COMMAND__CREATE_ACTION_COMMAND__H
#define SERVER_COMMAND__CREATE_ACTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateActionRequest.h>
#include <server/io/output/Action.h>

#include "commons.h"

namespace server {
namespace command {

class CreateActionCommand {
public:
    CreateActionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateActionCommand);

    io::output::Action operator()(const io::input::CreateActionRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateActionRequest &input) const;

    db::Database &db;
};
}
}

#endif