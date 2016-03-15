#ifndef SERVER_COMMAND__CREATE_ACTION_COMMAND__H
#define SERVER_COMMAND__CREATE_ACTION_COMMAND__H

#include <db/Database.h>

#include <server/io/input/CreateActionRequest.h>
#include <server/io/output/Action.h>

#include "Command.h"

namespace server {
namespace command {

class CreateActionCommand : public Command {
public:
    CreateActionCommand(db::Database &db);

    io::output::Action operator()(const io::input::CreateActionRequest &input);

private:
    void validateInput(const io::input::CreateActionRequest &input) const;

    db::Database &db;
};
}
}

#endif