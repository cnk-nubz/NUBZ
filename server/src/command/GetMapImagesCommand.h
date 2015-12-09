#ifndef COMMAND__GET_MAP_IMAGES_COMMAND__H
#define COMMAND__GET_MAP_IMAGES_COMMAND__H

#include "commands_common.h"
#include "io/input/MapImagesRequest.h"
#include "io/output/MapImagesResponse.h"
#include "db/Database.h"

namespace command {
    class GetMapImagesCommand {
    public:
        static void setUrlPathPrefix(const std::string &urlPrefix);

        GetMapImagesCommand(db::Database &db);
        ~GetMapImagesCommand() = default;

        DEFAULT_CP_MV(GetMapImagesCommand);

        io::output::MapImagesResponse operator()(const io::input::MapImagesRequest &input);

    private:
        static std::string urlPathPrefix;
        db::Database &db;

        std::string createFullUrl(const std::string &imgFileName) const;
    };
}

#endif