#ifndef SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H
#define SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H

#include <boost/optional.hpp>

#include <db/Database.h>
#include <db/struct/MapFrame.h>

#include <server/io/input/CreateExhibitRequest.h>
#include <server/io/Exhibit.h>

#include "commons.h"

namespace server {
namespace command {

class CreateExhibitCommand {
public:
    CreateExhibitCommand(db::Database &db);
    SRV_CMD_CP_MV(CreateExhibitCommand);

    io::Exhibit operator()(const io::input::CreateExhibitRequest &input);

private:
    db::Database &db;

    void validateInput(db::DatabaseSession &session,
                       const io::input::CreateExhibitRequest &input) const;
    db::MapFrame createExhibitFrame(std::int32_t dstFloor,
                                    const boost::optional<io::MapFrame> &visibleFrame) const;

    std::int32_t getNextVersion(db::DatabaseSession &session) const;
};
}
}

#endif