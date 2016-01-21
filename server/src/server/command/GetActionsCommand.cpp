#include <db/command/GetActions.h>

#include <server/utils/io_translation.h>

#include "GetActionsCommand.h"

namespace server {
namespace command {

GetActionsCommand::GetActionsCommand(db::Database &db) : db(db) {
}

io::output::ActionsResponse GetActionsCommand::operator()() {
    db::cmd::GetActions getActions;
    db.execute(getActions);

    std::vector<db::Action> actions = getActions.getResult();
    std::sort(actions.begin(), actions.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.text < rhs.text;
    });

    io::output::ActionsResponse output;
    for (const auto &action : actions) {
        output.actions.push_back(utils::toIO(action));
    }
    return output;
}
}
}