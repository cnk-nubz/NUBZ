#ifndef SERVER_COMMAND__ACTION_COMMANDS__H
#define SERVER_COMMAND__ACTION_COMMANDS__H

#include <vector>

#include <db/Database.h>

#include <server/io/input/CreateActionRequest.h>
#include <server/io/output/Action.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class ActionCommands : public Command {
public:
    ActionCommands(db::Database &db);

    Action create(const CreateActionRequest &input);
    std::vector<Action> getAll();

    void remove(std::int32_t actionID);

private:
    db::Database &db;
};
}
}

#endif