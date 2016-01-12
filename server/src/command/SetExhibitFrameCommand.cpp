#include "SetExhibitFrameCommand.h"
#include "InputChecker.h"
#include "io/InvalidInput.h"
#include "db/command/GetExhibits.h"
#include "db/command/GetCounter.h"
#include "db/command/SetCounter.h"
#include "db/command/UpdateExhibit.h"

namespace command {
    SetExhibitFrameCommand::SetExhibitFrameCommand(db::Database &db) : db(db) {
    }

    // function can only alter existing exhibit frame
    // if exhibit has no floor InvalidData will be thrown
    void SetExhibitFrameCommand::operator()(const io::input::SetExhibitFrameRequest &input) {
        db.execute([&](db::DatabaseSession &session) {
            validateInput(session, input);

            // download data
            db::cmd::GetExhibits getExhibit(input.exhibitId);
            getExhibit(session);
            db::cmd::GetCounter getCounter(db::info::counters::element_type::exhibits);
            getCounter(session);

            // prepare new
            db::Exhibit newExhibit = getExhibit.getResult().front();
            newExhibit.frame.value().x = input.newX;
            newExhibit.frame.value().y = input.newY;
            newExhibit.frame.value().width = input.newWidth;
            newExhibit.frame.value().height = input.newHeight;
            newExhibit.version = getCounter.getResult() + 1;

            // update
            db::cmd::SetCounter{db::info::counters::element_type::exhibits,
                                newExhibit.version}(session);
            db::cmd::UpdateExhibit{newExhibit}(session);
        });
    }

    void SetExhibitFrameCommand::validateInput(
        db::DatabaseSession &session, const io::input::SetExhibitFrameRequest &input) const {
        db::cmd::GetExhibits getExhibit(input.exhibitId);
        getExhibit(session);

        if (getExhibit.getResult().empty()) {
            throw io::InvalidInput("incorrect exhibit id");
        }

        db::Exhibit exhibit = getExhibit.getResult().front();

        if (!exhibit.frame) {
            throw io::InvalidInput("given exhibit doesn't belong to any floor");
        }

        db::MapElementFrame frame = exhibit.frame.value();

        InputChecker checker(session);
        if (!checker.checkExhibitFrame(
                frame.mapLevel, input.newX, input.newY, input.newWidth, input.newHeight)) {
            throw io::InvalidInput("incorrect frame");
        }
    }
}