#include <utils/fp_algorithm.h>

#include <db/command/GetExhibits.h>

#include <server/utils/io_translation.h>

#include "GetAllExhibitsCommand.h"

namespace server {
namespace command {

GetAllExhibitsCommand::GetAllExhibitsCommand(db::Database &db) : db(db) {
}

std::vector<io::Exhibit> GetAllExhibitsCommand::operator()() {
    db::cmd::GetExhibits getExhibits;
    db.execute(getExhibits);

    std::vector<db::Exhibit> dbExhibits = getExhibits.getResult();
    std::sort(dbExhibits.begin(), dbExhibits.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name < rhs.name;
    });

    std::vector<io::Exhibit> exhibits;
    ::utils::transform(dbExhibits, exhibits, [](const auto &e) { return utils::toIO(e); });
    return exhibits;
}
}
}