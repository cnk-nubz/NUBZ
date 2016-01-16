#include "CreateNewExhibitCommand.h"
#include "io/InvalidInput.h"
#include "db/struct/Exhibit.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetCounter.h"
#include "db/command/SetCounter.h"
#include "db/command/InsertExhibit.h"
#include "InputChecker.h"

namespace command {
    CreateNewExhibitCommand::CreateNewExhibitCommand(db::Database &db) : db(db) {
    }

    io::output::NewExhibitResponse CreateNewExhibitCommand::operator()(
        const io::input::NewExhibitRequest &input) {
        io::output::NewExhibitResponse output;
        db.execute([&](db::DatabaseSession &session) {
            validateInput(session, input);

            db::Exhibit exhibit;
            exhibit.version = getNextVersion(session);
            exhibit.name = input.name;
            if (input.floor) {
                exhibit.frame = createExhibitFrame(input.floor.value(), input.visibleMapFrame);
            }

            db::cmd::InsertExhibit insert(exhibit);
            insert(session);
            exhibit.ID = insert.getId();

            output.exhibit = io::Exhibit(exhibit);
            output.exhibitId = exhibit.ID;
        });
        return output;
    }

    void CreateNewExhibitCommand::validateInput(db::DatabaseSession &session,
                                                const io::input::NewExhibitRequest &input) const {
        InputChecker checker(session);
        if (!checker.checkText(input.name)) {
            throw io::InvalidInput("incorrect name");
        }

        if (input.visibleMapFrame) {
            auto visibleFrame = input.visibleMapFrame.value();
            if (!checker.checkFrame(visibleFrame.mapLevel,
                                    visibleFrame.x,
                                    visibleFrame.y,
                                    visibleFrame.width,
                                    visibleFrame.height)) {
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

    db::MapElementFrame CreateNewExhibitCommand::createExhibitFrame(
        std::int32_t dstFloor, const boost::optional<io::MapElementFrame> &visibleFrame) const {
        static const std::int32_t minSize = 200;
        static const std::int32_t screenProportion = 3;

        db::MapElementFrame frame;
        frame.mapLevel = dstFloor;
        if (!visibleFrame) {
            frame.x = frame.y = 0;
            frame.width = frame.height = minSize + rand() % minSize;
        } else {
            auto screenFrame = visibleFrame.value();
            frame.width = std::max(screenFrame.width / screenProportion, minSize);
            frame.height = std::max(screenFrame.height / screenProportion, minSize);

            std::int32_t middleX = screenFrame.x + screenFrame.width / 2;
            std::int32_t middleY = screenFrame.y + screenFrame.height / 2;
            frame.x = std::max(0, middleX - frame.width / 2);
            frame.y = std::max(0, middleY - frame.height / 2);
        }
        return frame;
    }

    std::int32_t CreateNewExhibitCommand::getNextVersion(db::DatabaseSession &session) const {
        using namespace db::info::counters;
        db::cmd::GetCounter getCounter(element_type::exhibits);
        getCounter(session);
        std::int32_t newId = getCounter.getResult() + 1;

        db::cmd::SetCounter{element_type::exhibits, newId}(session);
        return newId;
    }
}