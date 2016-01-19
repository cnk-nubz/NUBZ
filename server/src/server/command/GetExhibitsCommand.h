#ifndef SERVER_COMMAND__GET_EXHIBITS_COMMAND__H
#define SERVER_COMMAND__GET_EXHIBITS_COMMAND__H

#include <db/Database.h>

#include <server/io/input/ExhibitsRequest.h>
#include <server/io/output/ExhibitsResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetExhibitsCommand {
public:
    GetExhibitsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetExhibitsCommand);

    io::output::ExhibitsResponse operator()(const io::input::ExhibitsRequest &input);

private:
    db::Database &db;
};
}
}

#endif