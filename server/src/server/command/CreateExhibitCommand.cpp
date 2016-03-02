#include <db/command/GetMapImages.h>
#include <db/command/IncrementCounter.h>
#include <db/command/InsertExhibit.h>
#include <db/struct/Exhibit.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>
#include <server/utils/io_translation.h>

#include "CreateExhibitCommand.h"

namespace server {
namespace command {

CreateExhibitCommand::CreateExhibitCommand(db::Database &db) : db(db) {
}

io::Exhibit CreateExhibitCommand::operator()(const io::input::CreateExhibitRequest &input) {
    io::Exhibit exhibit;
    db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        db::Exhibit dbExhibit;
        dbExhibit.name = input.name;
        if (input.floor) {
            dbExhibit.frame = createExhibitFrame(input.floor.value(), input.visibleFrame);
        }
        dbExhibit.version = db::cmd::IncrementCounter::exhibitVersion()(session);
        dbExhibit.ID = db::cmd::InsertExhibit{dbExhibit}(session);

        exhibit = utils::toIO(dbExhibit);
    });
    return exhibit;
}

void CreateExhibitCommand::validateInput(db::DatabaseSession &session,
                                         const io::input::CreateExhibitRequest &input) const {
    utils::InputChecker checker(session);
    if (!checker.checkText(input.name)) {
        throw io::InvalidInput("incorrect name");
    }

    if (input.visibleFrame) {
        auto visibleFrame = input.visibleFrame.value();
        if (!checker.checkFrame(visibleFrame.floor,
                                visibleFrame.frame.x,
                                visibleFrame.frame.y,
                                visibleFrame.frame.size.width,
                                visibleFrame.frame.size.height)) {
            throw io::InvalidInput("incorrect frame");
        }
    }

    if (input.floor) {
        db::cmd::GetMapImages getMapImages(input.floor.value());
        getMapImages(session);
        if (getMapImages.getResult().empty()) {
            throw io::InvalidInput("floor without image and therefore without frame");
        }
    }
}

db::MapFrame CreateExhibitCommand::createExhibitFrame(
    std::int32_t dstFloor, const boost::optional<io::MapFrame> &visibleFrame) const {
    static const std::int32_t minSize = 200;
    static const std::int32_t screenProportion = 3;

    db::MapFrame frame;
    frame.floor = dstFloor;
    if (!visibleFrame || visibleFrame.value().floor != dstFloor) {
        frame.x = frame.y = 0;
        frame.width = frame.height = minSize + rand() % minSize;
    } else {
        auto screenFrame = visibleFrame.value().frame;
        frame.width = std::max(screenFrame.size.width / screenProportion, minSize);
        frame.height = std::max(screenFrame.size.height / screenProportion, minSize);

        std::int32_t middleX = screenFrame.x + screenFrame.size.width / 2;
        std::int32_t middleY = screenFrame.y + screenFrame.size.height / 2;
        frame.x = std::max(0, middleX - frame.width / 2);
        frame.y = std::max(0, middleY - frame.height / 2);
    }
    return frame;
}
}
}