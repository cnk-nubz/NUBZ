#ifndef COMMAND__CREATE_NEW_EXHIBIT_COMMAND__H
#define COMMAND__CREATE_NEW_EXHIBIT_COMMAND__H

#include <boost/optional.hpp>

#include "commands_common.h"
#include "io/input/NewExhibitRequest.h"
#include "io/output/NewExhibitResponse.h"
#include "db/Database.h"
#include "db/struct/MapElementFrame.h"

namespace command {
    class CreateNewExhibitCommand {
    public:
        CreateNewExhibitCommand(db::Database &db);
        ~CreateNewExhibitCommand() = default;

        DEFAULT_CP_MV(CreateNewExhibitCommand);

        io::output::NewExhibitResponse operator()(const io::input::NewExhibitRequest &input);

    private:
        db::Database &db;

        void validateInput(db::DatabaseSession &session,
                           const io::input::NewExhibitRequest &input) const;
        db::MapElementFrame createExhibitFrame(
            std::int32_t dstFloor, const boost::optional<io::MapElementFrame> &visibleFrame) const;

        std::int32_t getNextVersion(db::DatabaseSession &session) const;
    };
}

#endif