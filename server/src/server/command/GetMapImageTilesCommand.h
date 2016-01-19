#ifndef SERVER_COMMAND__GET_MAP_IMAGE_TILES_COMMAND__H
#define SERVER_COMMAND__GET_MAP_IMAGE_TILES_COMMAND__H

#include <db/Database.h>
#include <db/struct/MapTilesInfo.h>

#include <server/io/input/MapImageTilesRequest.h>
#include <server/io/output/MapImageTilesResponse.h>
#include <server/io/output/ImageTiles.h>

#include "commons.h"

namespace server {
namespace command {

class GetMapImageTilesCommand {
public:
    static void setUrlPathPrefix(const std::string &urlPrefix);

    GetMapImageTilesCommand(::db::Database &db);
    SRV_CMD_CP_MV(GetMapImageTilesCommand);

    io::output::MapImageTilesResponse operator()(const io::input::MapImageTilesRequest &input);

private:
    static std::string urlPathPrefix;
    db::Database &db;

    // temporary data, correct only inside operator()
    std::int32_t floor;
    io::output::MapImageTilesResponse *answer;

    void getOriginalSize(::db::DatabaseSession &session);
    void getZoomLevels(::db::DatabaseSession &session);
    io::output::ImageTiles getTiles(::db::DatabaseSession &session,
                                    const db::MapTilesInfo &tilesInfo);

    std::string createFullUrl(const std::string &imgFileName) const;
};
}
}

#endif