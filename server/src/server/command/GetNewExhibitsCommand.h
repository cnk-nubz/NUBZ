#ifndef SERVER_COMMAND__GET_NEW_EXHIBITS_COMMAND__H
#define SERVER_COMMAND__GET_NEW_EXHIBITS_COMMAND__H

#include <db/Database.h>

#include <server/io/input/NewExhibitsRequest.h>
#include <server/io/output/NewExhibitsResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetNewExhibitsCommand {
public:
    GetNewExhibitsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetNewExhibitsCommand);

    io::output::NewExhibitsResponse operator()(const io::input::NewExhibitsRequest &input);

private:
    db::Database &db;
};
}
}

#endif