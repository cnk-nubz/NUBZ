#ifndef SERVER_COMMAND__GET_NEW_MAP_IMAGES_COMMAND__H
#define SERVER_COMMAND__GET_NEW_MAP_IMAGES_COMMAND__H

#include <db/Database.h>

#include <server/io/input/NewMapImagesRequest.h>
#include <server/io/output/NewMapImagesResponse.h>

#include "commons.h"

namespace server {
namespace command {

class GetNewMapImagesCommand {
public:
    static void setUrlPathPrefix(const std::string &urlPrefix);

    GetNewMapImagesCommand(db::Database &db);
    SRV_CMD_CP_MV(GetNewMapImagesCommand);

    io::output::NewMapImagesResponse operator()(const io::input::NewMapImagesRequest &input);

private:
    static std::string urlPathPrefix;
    db::Database &db;

    std::string createFullUrl(const std::string &imgFileName) const;
};
}
}

#endif