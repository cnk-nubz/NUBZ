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
    GetNewMapImagesCommand(db::Database &db);
    SRV_CMD_CP_MV(GetNewMapImagesCommand);

    io::output::NewMapImagesResponse operator()(const io::input::NewMapImagesRequest &input);

private:
    db::Database &db;
};
}
}

#endif