#include <utils/fp_algorithm.h>

#include <repository/Exhibits.h>

#include <server/io/utils.h>
#include <server/utils/io_translation.h>

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
    return ::server::io::repoToIO<io::output::Exhibit>(repoExhibits);
}
}
}