#include "GetExhibitsCommand.h"
#include "db/command/GetExhibits.h"
#include "db/command/GetVersion.h"

namespace command {
    GetExhibitsCommand::GetExhibitsCommand(db::Database &db) : db(db) {
    }

    io::output::ExhibitsResponse GetExhibitsCommand::perform(
        const io::input::ExhibitsRequest &input) {
        db::cmd::GetVersion getVersion(db::info::versions::element_type::exhibits);
        std::unique_ptr<db::cmd::GetExhibits> getExhibits;
        if (input.acquiredVersion) {
            getExhibits.reset(new db::cmd::GetExhibits(*input.acquiredVersion + 1));
        } else {
            getExhibits.reset(new db::cmd::GetExhibits);
        }

        db.execute(getVersion);
        db.execute(*getExhibits);

        io::output::ExhibitsResponse response;
        response.version = getVersion.getResult();
        for (const auto &exhibit : getExhibits->getResult()) {
            response.exhibits[exhibit.ID] = io::Exhibit(exhibit);
        }

        return response;
    }
}