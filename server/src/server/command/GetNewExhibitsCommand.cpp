#include <tuple>

#include <repository/Counters.h>
#include <repository/Exhibits.h>

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
        auto exhibitsRepo = repository::Exhibits{session};
        auto countersRepo = repository::Counters{session};

        auto curVersion = countersRepo.get(repository::CounterType::LastExhibitVersion);
        auto exhibits = std::vector<repository::Exhibit>{};
        if (auto userVersion = input.acquiredVersion) {
            exhibits = exhibitsRepo.getAllNewerThan(userVersion.value());
        } else {
            exhibits = exhibitsRepo.getAll();
        }
        return std::make_tuple(curVersion, exhibits);
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