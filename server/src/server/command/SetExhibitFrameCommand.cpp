#include "SetExhibitFrameCommand.h"

#include <db/command/GetExhibits.h>
#include <db/command/IncrementCounter.h>
#include <db/command/UpdateExhibit.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

namespace server {
namespace command {

SetExhibitFrameCommand::SetExhibitFrameCommand(db::Database &db) : db(db) {
}

// function can only alter existing exhibit frame
// if exhibit has no floor InvalidData will be thrown
void SetExhibitFrameCommand::operator()(const io::input::SetExhibitFrameRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        // download data
        db::Exhibit exhibit = db::cmd::GetExhibits{input.exhibitId}(session).front();

        // prepare new
        // map level doesn't change
        exhibit.frame.value().x = input.frame.x;
        exhibit.frame.value().y = input.frame.y;
        exhibit.frame.value().width = input.frame.size.width;
        exhibit.frame.value().height = input.frame.size.height;
        exhibit.version = db::cmd::IncrementCounter::exhibitVersion()(session);

        // update
        db::cmd::UpdateExhibit{exhibit}(session);
    });
}

void SetExhibitFrameCommand::validateInput(db::DatabaseSession &session,
                                           const io::input::SetExhibitFrameRequest &input) const {
    db::cmd::GetExhibits getExhibit(input.exhibitId);

    getExhibit(session);
    if (getExhibit.getResult().empty()) {
        throw io::InvalidInput("incorrect exhibit id");
    }

    db::Exhibit exhibit = getExhibit.getResult().front();
    db::MapFrame frame = exhibit.frame.value();

    if (!exhibit.frame) {
        throw io::InvalidInput("given exhibit doesn't belong to any floor");
    }

    utils::InputChecker checker(session);
    if (!checker.checkFrame(frame.floor,
                            input.frame.x,
                            input.frame.y,
                            input.frame.size.width,
                            input.frame.size.height)) {
        throw io::InvalidInput("incorrect frame");
    }
}
}
}