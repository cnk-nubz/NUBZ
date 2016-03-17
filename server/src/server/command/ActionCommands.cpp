#include <algorithm>

#include <repository/Actions.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "ActionCommands.h"

namespace server {
namespace command {

ActionCommands::ActionCommands(db::Database &db) : db(db) {
}

Action ActionCommands::create(const CreateActionRequest &input) {
    if (!utils::checkText(input.text)) {
        throw io::InvalidInput{"name contains invalid characters"};
    }

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
        return std::make_pair(lhs.text, lhs.ID) < std::make_pair(rhs.text, rhs.ID);
    });
    return std::vector<Action>(repoActions.begin(), repoActions.end());
}
}
}