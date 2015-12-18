#ifndef COMMAND__SET_MAP_IMAGE_COMMAND__H
#define COMMAND__SET_MAP_IMAGE_COMMAND__H

#include "commands_common.h"
#include "io/input/SetMapImageRequest.h"
#include "db/Database.h"
#include "db/struct/MapImage.h"
#include "db/struct/MapTile.h"
#include "db/struct/MapTilesInfo.h"
#include "image/ImageProcessor.h"
#include "utils/FileHandler.h"

namespace command {
    class SetMapImageCommand {
    public:
        SetMapImageCommand(db::Database &db);
        ~SetMapImageCommand() = default;

        DEFAULT_CP_MV(SetMapImageCommand);

        void operator()(const io::input::SetMapImageRequest &input);

    private:
        db::Database &db;
        std::vector<db::MapTile> tiles;
        std::vector<db::MapTilesInfo> tilesInfo;
        int randVal;

        std::int32_t getCurrentVersion(db::DatabaseSession &session) const;
        std::string createFilename(const std::string &srcFilename, std::int32_t level) const;
        void setSize(db::MapImage &mapImage, const std::string imgPath) const;
        void moveFileToPublic(const std::string &srcFilename,
                              const std::string &destFilename) const;
        std::vector<utils::FileHandler> createTiles(const std::string &filePath,
                                                    std::int32_t floor);
        std::string getFilename(const std::string &path) const;
    };
}

#endif
