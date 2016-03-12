#include <repository/Counters.h>
#include <repository/MapImages.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "CreateExhibitCommand.h"

namespace server {
namespace command {

CreateExhibitCommand::CreateExhibitCommand(db::Database &db) : db(db) {
}

io::output::Exhibit CreateExhibitCommand::operator()(const io::input::CreateExhibitRequest &input) {
    auto repoExhibit = db.execute([&](db::DatabaseSession &session) {
        validateInput(session, input);

        auto exhibit = repository::Exhibit{};
        exhibit.name = input.name;
        if (input.floor) {
            exhibit.frame = createExhibitFrame(input.floor.value(), input.visibleFrame);
        }

        auto exhibitsRepo = repository::Exhibits{session};
        auto countersRepo = repository::Counters{session};
        exhibit.version = countersRepo.increment(repository::CounterType::LastExhibitVersion);
        exhibitsRepo.insert(&exhibit);
        return exhibit;
    });
    return io::output::Exhibit{repoExhibit};
}

void CreateExhibitCommand::validateInput(db::DatabaseSession &session,
                                         const io::input::CreateExhibitRequest &input) const {
    if (!utils::checkText(input.name)) {
        throw io::InvalidInput("incorrect name");
    }
}

repository::Exhibit::Frame CreateExhibitCommand::createExhibitFrame(
    std::int32_t dstFloor, const boost::optional<io::MapFrame> &visibleFrame) const {
    static const std::int32_t minSize = 200;
    static const std::int32_t screenProportion = 3;

    auto frame = repository::Exhibit::Frame{};
    frame.floor = dstFloor;
    if (!visibleFrame || visibleFrame.value().floor != dstFloor) {
        frame.x = frame.y = 0;
        frame.width = frame.height = minSize;
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