#include <utils/fp_algorithm.h>

#include <db/command/GetActions.h>

#include <server/utils/io_translation.h>

#include "GetAllActionsCommand.h"

namespace server {
namespace command {

GetAllActionsCommand::GetAllActionsCommand(db::Database &db) : db(db) {
}

std::vector<io::Action> GetAllActionsCommand::operator()() {
    db::cmd::GetActions getActions;
    db.execute(getActions);

    std::vector<db::Action> dbActions = getActions.getResult();
    std::sort(dbActions.begin(),
              dbActions.end(),
              [](const auto &lhs, const auto &rhs) { return lhs.text < rhs.text; });

    std::vector<io::Action> actions;
    ::utils::transform(dbActions, actions, [](const auto &a) { return utils::toIO(a); });
    return actions;
}
}
}