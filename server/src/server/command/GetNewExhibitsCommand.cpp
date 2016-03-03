#include <tuple>

#include <repository/Exhibits.h>

#include <db/command/GetCounter.h>
#include <db/command/GetRawReports.h>

#include <server/io/utils.h>

#include "GetNewExhibitsCommand.h"

namespace server {
namespace command {

GetNewExhibitsCommand::GetNewExhibitsCommand(db::Database &db) : db(db) {
}

io::output::NewExhibitsResponse GetNewExhibitsCommand::operator()(
    const io::input::NewExhibitsRequest &input) {
    std::vector<repository::Exhibit> repoExhibits;
    std::int32_t currentVersion;

    std::tie(currentVersion, repoExhibits) = db.execute([&](db::DatabaseSession &session) {
        std::int32_t curVersion =
            db::cmd::GetCounter{db::info::counters::element_type::exhibits}(session);
        auto repo = repository::Exhibits{session};

        if (auto userVersion = input.acquiredVersion) {
            auto repoOut = repo.getAllNewerThan(userVersion.value());
            return std::make_tuple(curVersion, repoOut);
        } else {
            return std::make_tuple(curVersion, repo.getAll());
        }
    });

    auto exhibits = ::server::io::repoToIO<io::Exhibit>(repoExhibits);

    auto response = io::output::NewExhibitsResponse{};
    response.version = currentVersion;
    for (auto &exhibit : exhibits) {
        response.exhibits[exhibit.ID] = exhibit;
    }
    return response;
}
}
}