#ifndef SERVER_COMMAND__SET_EXHIBIT_FRAME_COMMAND__H
#define SERVER_COMMAND__SET_EXHIBIT_FRAME_COMMAND__H

#include <db/Database.h>

#include <server/io/input/SetExhibitFrameRequest.h>

#include "Command.h"

namespace server {
namespace command {

class SetExhibitFrameCommand : public Command {
public:
    SetExhibitFrameCommand(db::Database &db);

    void operator()(const io::input::SetExhibitFrameRequest &input);

private:
    void validateInput(db::DatabaseSession &session,
                       const io::input::SetExhibitFrameRequest &input) const;

    db::Database &db;
};
}
}

#endif