#ifndef SERVER_COMMAND__GET_NEW_EXHIBITS_COMMAND__H
#define SERVER_COMMAND__GET_NEW_EXHIBITS_COMMAND__H

#include <db/Database.h>

#include <server/io/input/NewExhibitsRequest.h>
#include <server/io/output/NewExhibitsResponse.h>

#include "Command.h"

namespace server {
namespace command {

class GetNewExhibitsCommand : public Command {
public:
    GetNewExhibitsCommand(db::Database &db);

    io::output::NewExhibitsResponse operator()(const io::input::NewExhibitsRequest &input);

private:
    db::Database &db;
};
}
}

#endif