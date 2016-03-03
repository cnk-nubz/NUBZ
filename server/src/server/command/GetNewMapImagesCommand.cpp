#include <repository/Counters.h>

#include <db/command/GetMapImages.h>

#include "GetNewMapImagesCommand.h"

namespace server {
namespace command {

std::string GetNewMapImagesCommand::urlPathPrefix = "";

void GetNewMapImagesCommand::setUrlPathPrefix(const std::string &urlPrefix) {
    assert(urlPrefix.size() > 0);
    urlPathPrefix = urlPrefix;
    if (urlPathPrefix.back() != '/') {
        urlPathPrefix += "/";
    }
}

GetNewMapImagesCommand::GetNewMapImagesCommand(db::Database &db) : db(db) {
}

io::output::NewMapImagesResponse GetNewMapImagesCommand::operator()(
    const io::input::NewMapImagesRequest &input) {
    auto mapImages = std::vector<db::MapImage>{};
    std::int32_t version;
    std::tie(version, mapImages) = db.execute([&](db::DatabaseSession &session) {
        auto getMapImages = db::cmd::GetMapImages{};
        if (input.acquiredVersion) {
            getMapImages.minVersion = *input.acquiredVersion + 1;
        }
        getMapImages(session);
        auto mapImages = getMapImages.getResult();

        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.get(repository::CounterType::LastMapVersion);

        return std::make_tuple(version, mapImages);
    });

    auto response = io::output::NewMapImagesResponse{};
    response.version = version;
    for (const auto &mapImage : mapImages) {
        response.floorImageUrls[mapImage.floor] = createFullUrl(mapImage.filename);
    }
    return response;
}

std::string GetNewMapImagesCommand::createFullUrl(const std::string &imgFileName) const {
    assert(urlPathPrefix.size() > 0);
    return urlPathPrefix + imgFileName;
}
}
}