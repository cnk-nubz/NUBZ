#include <algorithm>

#include <repository/Actions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/CmpUTF8.h>

#include "ActionCommands.h"

namespace server {
namespace command {

ActionCommands::ActionCommands(db::Database &db) : db(db) {
}

Action ActionCommands::create(const CreateActionRequest &input) {
    auto action = repository::Action{};
    action.text = input.text;
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Actions{session};
        repo.insert(&action);
    });
    return Action{action};
}

std::vector<Action> ActionCommands::getAll() {
    auto repoActions = db.execute(
        [&](db::DatabaseSession &session) { return repository::Actions{session}.getAll(); });

    std::sort(repoActions.begin(), repoActions.end(), [](auto &lhs, auto &rhs) {
        return utils::cmpUTF8(lhs.text, rhs.text);
    });
    return std::vector<Action>(repoActions.begin(), repoActions.end());
}
}
}