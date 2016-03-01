#include <utils/fp_algorithm.h>

#include <repository/Actions.h>

#include <server/utils/io_translation.h>

#include "GetAllActionsCommand.h"

namespace server {
namespace command {

GetAllActionsCommand::GetAllActionsCommand(db::Database &db) : db(db) {
}

std::vector<io::Action> GetAllActionsCommand::operator()() {
    auto dbActions = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Actions{session};
        return repo.getAll();
    });

    std::sort(dbActions.begin(),
              dbActions.end(),
              [](auto &lhs, auto &rhs) {
                  return std::make_pair(lhs.text, lhs.ID) < std::make_pair(rhs.text, rhs.ID);
              });

    return utils::toIO<io::Action>(dbActions);
}
}
}