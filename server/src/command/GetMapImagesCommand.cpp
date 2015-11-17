#include "GetMapImagesCommand.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetVersion.h"

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

    io::output::MapImagesResponse GetMapImagesCommand::perform(
        const io::input::MapImagesRequest &input) {
        db::cmd::GetVersion getVersion(db::cmd::GetVersion::ElementType::MapImage);
        std::unique_ptr<db::cmd::GetMapImages> getMapImages;
        if (input.acquiredLevel) {
            getMapImages.reset(new db::cmd::GetMapImages(*input.acquiredLevel + 1));
        } else {
            getMapImages.reset(new db::cmd::GetMapImages);
        }

        db.execute(&getVersion);
        db.execute(getMapImages.get());

        io::output::MapImagesResponse response;
        response.version = getVersion.getResult();
        for (const auto &mapImage : getMapImages->getResult()) {
            response.levelImageUrls[mapImage.level] = createFullUrl(mapImage.filename);
        }

        return response;
    }

    std::string GetMapImagesCommand::createFullUrl(const std::string &imgFileName) const {
        assert(urlPathPrefix.size() > 0);
        return urlPathPrefix + imgFileName;
    }
}