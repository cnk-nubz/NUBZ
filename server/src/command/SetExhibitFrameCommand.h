#ifndef COMMAND__SET_EXHIBIT_FRAME_COMMAND__H
#define COMMAND__SET_EXHIBIT_FRAME_COMMAND__H

#include "commands_common.h"
#include "io/input/SetExhibitFrameRequest.h"
#include "db/Database.h"

namespace command {
    class SetExhibitFrameCommand {
    public:
        SetExhibitFrameCommand(db::Database &db);
        ~SetExhibitFrameCommand() = default;

        DEFAULT_CP_MV(SetExhibitFrameCommand);

        void operator()(const io::input::SetExhibitFrameRequest &input);

    private:
        db::Database &db;

        void validateInput(db::DatabaseSession &session,
                           const io::input::SetExhibitFrameRequest &input) const;
    };
}

#endif