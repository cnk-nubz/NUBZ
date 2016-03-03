#include <repository/Exhibits.h>

#include <db/command/IncrementCounter.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "SetExhibitFrameCommand.h"

namespace server {
namespace command {

SetExhibitFrameCommand::SetExhibitFrameCommand(db::Database &db) : db(db) {
}

// function can only alter existing exhibit frame
// if exhibit has no floor InvalidData will be thrown
void SetExhibitFrameCommand::operator()(const io::input::SetExhibitFrameRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        auto repo = repository::Exhibits{session};

        auto exhibit = repo.get(input.exhibitId).value();
        auto repoFrame = exhibit.frame.value();
        repoFrame.x = input.frame.x;
        repoFrame.y = input.frame.y;
        repoFrame.width = input.frame.size.width;
        repoFrame.height = input.frame.size.height;

        repo.setFrame(exhibit.ID, repoFrame);
        repo.setVersion(exhibit.ID, db::cmd::IncrementCounter::exhibitVersion()(session));
    });
}

void SetExhibitFrameCommand::validateInput(db::DatabaseSession &session,
                                           const io::input::SetExhibitFrameRequest &input) const {
    auto repo = repository::Exhibits{session};

    auto exhibitOpt = repo.get(input.exhibitId);
    if (!exhibitOpt) {
        throw io::InvalidInput("incorrect exhibit id");
    }
    auto exhibit = exhibitOpt.value();

    if (!exhibit.frame) {
        throw io::InvalidInput("given exhibit doesn't belong to any floor");
    }
    auto frame = exhibit.frame.value();

    auto checker = utils::InputChecker(session);
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