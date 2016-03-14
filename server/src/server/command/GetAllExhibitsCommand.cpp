#include <algorithm>

#include <repository/Exhibits.h>

#include "GetAllExhibitsCommand.h"

namespace server {
namespace command {

GetAllExhibitsCommand::GetAllExhibitsCommand(db::Database &db) : db(db) {
}

std::vector<io::output::Exhibit> GetAllExhibitsCommand::operator()() {
    auto repoExhibits = db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Exhibits{session};
        return repo.getAll();
    });

    std::sort(repoExhibits.begin(), repoExhibits.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name < rhs.name;
    });
    return std::vector<io::output::Exhibit>(repoExhibits.begin(), repoExhibits.end());
}
}
}