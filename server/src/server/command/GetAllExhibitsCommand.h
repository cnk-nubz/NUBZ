#ifndef SERVER_COMMAND__GET_ALL_EXHIBITS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_EXHIBITS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/Exhibit.h>

#include "commons.h"

namespace server {
namespace command {

class GetAllExhibitsCommand {
public:
    GetAllExhibitsCommand(db::Database &db);
    SRV_CMD_CP_MV(GetAllExhibitsCommand);

    std::vector<io::output::Exhibit> operator()();

private:
    db::Database &db;
};
}
}

#endif