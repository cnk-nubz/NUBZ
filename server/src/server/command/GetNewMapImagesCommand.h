#ifndef SERVER_COMMAND__GET_NEW_MAP_IMAGES_COMMAND__H
#define SERVER_COMMAND__GET_NEW_MAP_IMAGES_COMMAND__H

#include <db/Database.h>

#include <server/io/input/NewMapImagesRequest.h>
#include <server/io/output/NewMapImagesResponse.h>

#include "Command.h"

namespace server {
namespace command {

class GetNewMapImagesCommand : public Command {
public:
    GetNewMapImagesCommand(db::Database &db);

    io::output::NewMapImagesResponse operator()(const io::input::NewMapImagesRequest &input);

private:
    db::Database &db;
};
}
}

#endif