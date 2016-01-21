#include <db/command/InsertAction.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateNewActionCommand.h"

namespace server {
namespace command {

CreateNewActionCommand::CreateNewActionCommand(db::Database &db) : db(db) {
}

io::output::NewActionResponse CreateNewActionCommand::operator()(
    const io::input::NewActionRequest &input) {
    io::output::NewActionResponse output;

    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::Action newAction;
        newAction.text = input.text;
        newAction.ID = db::cmd::InsertAction{newAction}(session);
        output.action = utils::toIO(newAction);
    });

    return output;
}

void CreateNewActionCommand::validateInput(db::DatabaseSession &session,
                                           const io::input::NewActionRequest &input) const {
    utils::InputChecker checker(session);
    if (!checker.checkText(input.text)) {
        throw io::InvalidInput("incorrect name");
    }
}
}
}