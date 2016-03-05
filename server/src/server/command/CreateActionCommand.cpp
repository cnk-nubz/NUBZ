#include <db/command/InsertAction.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateActionCommand.h"

namespace server {
namespace command {

CreateActionCommand::CreateActionCommand(db::Database &db) : db(db) {
}

io::Action CreateActionCommand::operator()(const io::input::CreateActionRequest &input) {
    io::Action action;
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::Action dbAction;
        dbAction.text = input.text;
        dbAction.ID = db::cmd::InsertAction{dbAction}(session);
        action = utils::toIO(dbAction);
    });
    return action;
}

void CreateActionCommand::validateInput(db::DatabaseSession &session,
                                        const io::input::CreateActionRequest &input) const {
    utils::InputChecker checker(session);
    if (!checker.checkText(input.text)) {
        throw io::InvalidInput("incorrect name");
    }
}
}
}