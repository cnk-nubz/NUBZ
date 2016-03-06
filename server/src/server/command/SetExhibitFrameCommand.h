#ifndef SERVER_COMMAND__SET_EXHIBIT_FRAME_COMMAND__H
#define SERVER_COMMAND__SET_EXHIBIT_FRAME_COMMAND__H

#include <db/Database.h>

#include <server/io/input/SetExhibitFrameRequest.h>

#include "commons.h"

namespace server {
namespace command {

class SetExhibitFrameCommand {
public:
    SetExhibitFrameCommand(db::Database &db);
    SRV_CMD_CP_MV(SetExhibitFrameCommand);

    void operator()(const io::input::SetExhibitFrameRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::SetExhibitFrameRequest &input) const;

    db::Database &db;
};
}
}

#endif