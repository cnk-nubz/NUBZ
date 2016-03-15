#include <utils/fp_algorithm.h>

#include <repository/Actions.h>

#include <server/io/utils.h>

#include "GetAllActionsCommand.h"

namespace server {
namespace command {

GetAllActionsCommand::GetAllActionsCommand(db::Database &db) : db(db) {
}

std::vector<io::output::Action> GetAllActionsCommand::operator()() {
    auto repoActions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Actions{session};
        return repo.getAll();
    });

    std::sort(repoActions.begin(), repoActions.end(), [](auto &lhs, auto &rhs) {
        return std::make_pair(lhs.text, lhs.ID) < std::make_pair(rhs.text, rhs.ID);
    });
    return std::vector<io::output::Action>(repoActions.begin(), repoActions.end());
}
}
}