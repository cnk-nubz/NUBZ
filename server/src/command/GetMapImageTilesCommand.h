#ifndef COMMAND__GET_MAP_IMAGE_TILES_COMMAND__H
#define COMMAND__GET_MAP_IMAGE_TILES_COMMAND__H

#include "commands_common.h"
#include "io/input/MapImageTilesRequest.h"
#include "io/output/MapImageTilesResponse.h"
#include "io/output/ImageTiles.h"
#include "db/Database.h"
#include "db/struct/MapTilesInfo.h"

namespace command {
    class GetMapImageTilesCommand {
    public:
        static void setUrlPathPrefix(const std::string &urlPrefix);

        GetMapImageTilesCommand(db::Database &db);
        ~GetMapImageTilesCommand() = default;

        DEFAULT_CP_MV(GetMapImageTilesCommand);

        io::output::MapImageTilesResponse operator()(const io::input::MapImageTilesRequest &input);

    private:
        static std::string urlPathPrefix;
        db::Database &db;

        // temporary data, correct only inside operator()
        std::int32_t floor;
        io::output::MapImageTilesResponse *answer;

        void getOriginalSize(db::DatabaseSession &session);
        void getZoomLevels(db::DatabaseSession &session);
        io::output::ImageTiles getTiles(db::DatabaseSession &session,
                                        const db::MapTilesInfo &tilesInfo);

        std::string createFullUrl(const std::string &imgFileName) const;
    };
}

#endif