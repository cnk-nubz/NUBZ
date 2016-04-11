#ifndef SERVER_COMMAND__EXHIBIT_COMMANDS__H
#define SERVER_COMMAND__EXHIBIT_COMMANDS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/Database.h>

#include <repository/Exhibits.h>

#include <server/io/MapFrame.h>
#include <server/io/input/CreateExhibitRequest.h>
#include <server/io/input/NewExhibitsRequest.h>
#include <server/io/input/SetExhibitFrameRequest.h>
#include <server/io/input/UpdateExhibitRequest.h>
#include <server/io/output/Exhibit.h>
#include <server/io/output/NewExhibitsResponse.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class ExhibitCommands : public Command {
public:
    ExhibitCommands(db::Database &db);

    Exhibit create(const CreateExhibitRequest &input);

    std::vector<Exhibit> getAll();
    NewExhibitsResponse getNew(const NewExhibitsRequest &input);

    void setFrame(const SetExhibitFrameRequest &input);
    void update(const UpdateExhibitRequest &input);

    void remove(std::int32_t exhibitID);

private:
    boost::optional<repository::Exhibit::Frame> createFrame(
        const boost::optional<std::int32_t> &dstFloor,
        const boost::optional<io::MapFrame> &visibleFrame = {}) const;

    db::Database &db;
};
}
}

#endif