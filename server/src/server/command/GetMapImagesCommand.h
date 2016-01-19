#ifndef SERVER_COMMAND__GET_MAP_IMAGES_COMMAND__H
#define SERVER_COMMAND__GET_MAP_IMAGES_COMMAND__H

#include <db/Database.h>

#include <server/io/input/MapImagesRequest.h>
#include <server/io/output/MapImagesResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetMapImagesCommand {
public:
    static void setUrlPathPrefix(const std::string &urlPrefix);

    GetMapImagesCommand(db::Database &db);
    SRV_CMD_CP_MV(GetMapImagesCommand);

    io::output::MapImagesResponse operator()(const io::input::MapImagesRequest &input);

private:
    static std::string urlPathPrefix;
    db::Database &db;

    std::string createFullUrl(const std::string &imgFileName) const;
};
}
}

#endif