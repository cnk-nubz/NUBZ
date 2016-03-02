#ifndef SERVER_COMMAND__SET_MAP_IMAGE_COMMAND__H
#define SERVER_COMMAND__SET_MAP_IMAGE_COMMAND__H

#include <memory>

#include <boost/filesystem.hpp>

#include <db/Database.h>
#include <db/struct/MapImage.h>
#include <db/struct/MapTile.h>
#include <db/struct/MapTilesInfo.h>

#include <utils/FileHandler.h>
#include <utils/ImageProcessor.h>

#include <server/io/input/SetMapImageRequest.h>

#include "commons.h"

namespace server {
namespace command {

class SetMapImageCommand {
public:
    SetMapImageCommand(db::Database &db);
    SRV_CMD_CP_MV(SetMapImageCommand);

    void operator()(const io::input::SetMapImageRequest &input);

private:
    struct ZoomLevelInfo {
        std::int32_t levelIdx;
        std::size_t size;
        std::size_t tileSize;
    };
    static const std::vector<ZoomLevelInfo> zoomLevels;

    db::Database &db;
    std::unique_ptr<::utils::ImageProcessor> imgProc;
    std::vector<db::MapTile> tiles;
    std::vector<db::MapTilesInfo> tilesInfo;

    void prepareImageProcessor(const std::string &originalMapFilename);
    db::MapImage createFullMapImage(std::int32_t floor);

    ::utils::FileHandler createTiles(std::int32_t floor);
    void addTiles(const std::vector<std::vector<std::string>> &tilesNames, std::int32_t floor,
                  std::int32_t zoomLevel, const std::string &pathPrefix);

    boost::optional<boost::filesystem::path> getOldMap(std::int32_t floor,
                                                       db::DatabaseSession &session) const;

    void switchToNewTiles(const boost::filesystem::path &tmpDir, std::int32_t floor) const;

    boost::filesystem::path finalTilesDir(std::int32_t floor) const;
};
}
}

#endif
