#ifndef SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H
#define SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H

#include <boost/optional.hpp>

#include <repository/Exhibits.h>

#include <db/Database.h>

#include <server/io/Exhibit.h>
#include <server/io/input/CreateExhibitRequest.h>

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
    repository::Exhibit::Frame createExhibitFrame(
        std::int32_t dstFloor, const boost::optional<io::MapFrame> &visibleFrame) const;
};
}
}

#endif