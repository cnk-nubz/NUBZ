#ifndef SERVER_COMMAND__CREATE_NEW_ACTION_COMMAND__H
#define SERVER_COMMAND__CREATE_NEW_ACTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/NewActionRequest.h>
#include <server/io/output/NewActionResponse.h>

#include "commons.h"

namespace server {
namespace command {

class CreateNewActionCommand {
public:
    CreateNewActionCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateNewActionCommand);

    io::output::NewActionResponse operator()(const io::input::NewActionRequest &input);

private:
    db::Database &db;

    void validateInput(db::DatabaseSession &session,
                       const io::input::NewActionRequest &input) const;
};
}
}

#endif