#include "GetMapImageTilesCommand.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetMapTiles.h"
#include "db/command/GetMapTilesInfo.h"

namespace command {
    std::string GetMapImageTilesCommand::urlPathPrefix = "";

    void GetMapImageTilesCommand::setUrlPathPrefix(const std::string &urlPrefix) {
        assert(urlPrefix.size() > 0);
        urlPathPrefix = urlPrefix;
        if (urlPathPrefix.back() != '/') {
            urlPathPrefix += "/";
        }
    }

    GetMapImageTilesCommand::GetMapImageTilesCommand(db::Database &db) : db(db) {
    }

    io::output::MapImageTilesResponse GetMapImageTilesCommand::operator()(
        const io::input::MapImageTilesRequest &input) {
        io::output::MapImageTilesResponse response;

        answer = &response;
        floor = input.floor;
        db.execute([&](db::DatabaseSession &session) {
            getOriginalSize(session);
            getZoomLevels(session);
        });

        return response;
    }

    void GetMapImageTilesCommand::getOriginalSize(db::DatabaseSession &session) {
        db::cmd::GetMapImages getMapImages;
        getMapImages(session);
        for (const auto &mapImg : getMapImages.getResult()) {
            if (mapImg.floor == floor) {
                answer->originalSize.width = mapImg.width;
                answer->originalSize.height = mapImg.height;
                return;
            }
        }
        answer->originalSize.width = 0;
        answer->originalSize.height = 0;
    }

    void GetMapImageTilesCommand::getZoomLevels(db::DatabaseSession &session) {
        db::cmd::GetMapTilesInfo getTilesInfo{floor};
        getTilesInfo(session);

        std::vector<db::MapTilesInfo> tilesInfo = getTilesInfo.getResult();
        std::sort(tilesInfo.begin(),
                  tilesInfo.end(),
                  [](const db::MapTilesInfo &a, const db::MapTilesInfo &b) {
            return a.zoomLevel < b.zoomLevel;
        });

        for (const auto &tileInfo : tilesInfo) {
            answer->zoomLevels.push_back(getTiles(session, tileInfo));
        }
    }

    io::output::ImageTiles GetMapImageTilesCommand::getTiles(db::DatabaseSession &session,
                                                             const db::MapTilesInfo &tilesInfo) {
        io::output::ImageTiles imageTiles(tilesInfo.rowsCount, tilesInfo.colsCount);
        imageTiles.scaledSize.width = tilesInfo.imgWidth;
        imageTiles.scaledSize.height = tilesInfo.imgHeight;
        imageTiles.tileSize.width = tilesInfo.tileSize;
        imageTiles.tileSize.height = tilesInfo.tileSize;

        db::cmd::GetMapTiles getTilesCmd(floor, tilesInfo.zoomLevel);
        getTilesCmd(session);
        for (const auto &tile : getTilesCmd.getResult()) {
            imageTiles.tilesUrls[tile.row][tile.col] = createFullUrl(tile.filename);
        }
        return imageTiles;
    }

    std::string GetMapImageTilesCommand::createFullUrl(const std::string &imgFileName) const {
        assert(urlPathPrefix.size() > 0);
        return urlPathPrefix + imgFileName;
    }
}