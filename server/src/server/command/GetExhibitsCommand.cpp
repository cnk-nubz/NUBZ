#include <db/command/GetExhibits.h>
#include <db/command/GetCounter.h>
#include <db/command/GetRawReports.h>

#include <server/utils/io_translation.h>

#include "GetExhibitsCommand.h"

namespace server {
namespace command {

GetExhibitsCommand::GetExhibitsCommand(db::Database &db) : db(db) {
}

io::output::ExhibitsResponse GetExhibitsCommand::operator()(
    const io::input::ExhibitsRequest &input) {
    db::cmd::GetCounter getCounter(db::info::counters::element_type::exhibits);
    db::cmd::GetExhibits getExhibits;
    if (input.acquiredVersion) {
        getExhibits.minVersion = input.acquiredVersion.value() + 1;
    }

    db.execute(getCounter);
    db.execute(getExhibits);

    io::output::ExhibitsResponse response;
    response.version = getCounter.getResult();
    for (const auto &exhibit : getExhibits.getResult()) {
        response.exhibits[exhibit.ID] = utils::toIO(exhibit);
    }

    return response;
}
}
}