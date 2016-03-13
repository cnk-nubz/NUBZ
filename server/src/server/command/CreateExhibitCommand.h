#ifndef SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H
#define SERVER_COMMAND__CREATE_EXHIBIT_COMMAND__H

#include <boost/optional.hpp>

#include <db/Database.h>

#include <server/io/input/CreateExhibitRequest.h>
#include <server/io/output/Exhibit.h>

#include <repository/Exhibits.h>

#include "Command.h"

namespace server {
namespace command {

class CreateExhibitCommand : public Command {
public:
    CreateExhibitCommand(db::Database &db);

    io::output::Exhibit operator()(const io::input::CreateExhibitRequest &input);

private:
    void validateInput(const io::input::CreateExhibitRequest &input) const;
    repository::Exhibit::Frame createExhibitFrame(
        std::int32_t dstFloor, const boost::optional<io::MapFrame> &visibleFrame) const;

    db::Database &db;
};
}
}

#endif