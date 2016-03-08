#include <repository/Actions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "CreateActionCommand.h"

namespace server {
namespace command {

CreateActionCommand::CreateActionCommand(db::Database &db) : db(db) {
}

io::output::Action CreateActionCommand::operator()(const io::input::CreateActionRequest &input) {
    auto dbAction = db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        auto action = repository::Action{};
        action.text = input.text;

        auto repo = repository::Actions{session};
        repo.insert(&action);
        return action;
    });

    return io::output::Action{dbAction};
}

void CreateActionCommand::validateInput(db::DatabaseSession &session,
                                        const io::input::CreateActionRequest &input) const {
    auto checker = utils::InputChecker{session};
    if (!checker.checkText(input.text)) {
        throw io::InvalidInput("incorrect name");
    }
}
}
}