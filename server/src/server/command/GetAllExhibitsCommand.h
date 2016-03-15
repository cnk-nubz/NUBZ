#ifndef SERVER_COMMAND__GET_ALL_EXHIBITS_COMMAND__H
#define SERVER_COMMAND__GET_ALL_EXHIBITS_COMMAND__H

#include <db/Database.h>

#include <server/io/output/Exhibit.h>

#include "Command.h"

namespace server {
namespace command {

class GetAllExhibitsCommand : public Command {
public:
    GetAllExhibitsCommand(db::Database &db);

    std::vector<io::output::Exhibit> operator()();

private:
    db::Database &db;
};
}
}

#endif