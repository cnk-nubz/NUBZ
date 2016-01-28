#include <db/command/GetMapImages.h>
#include <db/command/GetCounter.h>

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
    db::cmd::GetCounter getCounter(db::info::counters::element_type::map_images);
    db::cmd::GetMapImages getMapImages;
    if (input.acquiredVersion) {
        getMapImages.minVersion = *input.acquiredVersion + 1;
    }

    db.execute([&](db::DatabaseSession &session) {
        getCounter(session);
        getMapImages(session);
    });

    io::output::NewMapImagesResponse response;
    response.version = getCounter.getResult();
    for (const auto &mapImage : getMapImages.getResult()) {
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