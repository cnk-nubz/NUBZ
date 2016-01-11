#include "GetMapImagesCommand.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetCounter.h"

namespace command {
    std::string GetMapImagesCommand::urlPathPrefix = "";

    void GetMapImagesCommand::setUrlPathPrefix(const std::string &urlPrefix) {
        assert(urlPrefix.size() > 0);
        urlPathPrefix = urlPrefix;
        if (urlPathPrefix.back() != '/') {
            urlPathPrefix += "/";
        }
    }

    GetMapImagesCommand::GetMapImagesCommand(db::Database &db) : db(db) {
    }

    io::output::MapImagesResponse GetMapImagesCommand::operator()(
        const io::input::MapImagesRequest &input) {
        db::cmd::GetCounter getCounter(db::info::counters::element_type::map_images);
        db::cmd::GetMapImages getMapImages;
        if (input.acquiredVersion) {
            getMapImages.minVersion = *input.acquiredVersion + 1;
        }

        db.execute(getCounter);
        db.execute(getMapImages);

        io::output::MapImagesResponse response;
        response.version = getCounter.getResult();
        for (const auto &mapImage : getMapImages.getResult()) {
            response.levelImageUrls[mapImage.floor] = createFullUrl(mapImage.filename);
        }

        return response;
    }

    std::string GetMapImagesCommand::createFullUrl(const std::string &imgFileName) const {
        assert(urlPathPrefix.size() > 0);
        return urlPathPrefix + imgFileName;
    }
}