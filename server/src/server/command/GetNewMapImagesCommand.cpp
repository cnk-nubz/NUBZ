#include <repository/Counters.h>
#include <repository/MapImages.h>

#include <server/utils/PathHelper.h>

#include "GetNewMapImagesCommand.h"

namespace server {
namespace command {

GetNewMapImagesCommand::GetNewMapImagesCommand(db::Database &db) : db(db) {
}

io::output::NewMapImagesResponse GetNewMapImagesCommand::operator()(
    const io::input::NewMapImagesRequest &input) {
    auto mapImages = std::vector<repository::MapImage>{};
    std::int32_t version;
    std::tie(version, mapImages) = db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto curVersion = countersRepo.get(repository::Counters::Type::LastMapVersion);

        auto imagesRepo = repository::MapImages{session};
        if (input.acquiredVersion) {
            auto usersVersion = input.acquiredVersion.value();
            return std::make_pair(curVersion, imagesRepo.getAllNewerThan(usersVersion));
        } else {
            return std::make_pair(curVersion, imagesRepo.getAll());
        }
    });

    auto result = io::output::NewMapImagesResponse{};
    result.version = version;
    for (const auto &mapImage : mapImages) {
        result.floors[mapImage.floor] = io::output::MapImage{mapImage};
    }
    return result;
}
}
}