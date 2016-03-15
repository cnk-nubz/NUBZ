#include <algorithm>

#include <repository/Counters.h>

#include <server/io/InvalidInput.h>
#include <server/utils/InputChecker.h>

#include "ExhibitCommands.h"

namespace server {
namespace command {

ExhibitCommands::ExhibitCommands(db::Database &db) : db(db) {
}

Exhibit ExhibitCommands::create(const CreateExhibitRequest &input) {
    auto repoExhibit = db.execute([&](db::DatabaseSession &session) {
        if (!server::utils::checkText(input.name)) {
            throw io::InvalidInput{"name contains invalid characters"};
        }

        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.increment(repository::CounterType::LastExhibitVersion);

        auto exhibit = repository::Exhibit{};
        exhibit.name = input.name;
        exhibit.frame = createFrame(input.floor, input.visibleFrame);
        exhibit.version = version;

        auto exhibitsRepo = repository::Exhibits{session};
        exhibitsRepo.insert(&exhibit);
        return exhibit;
    });

    return Exhibit{repoExhibit};
}

std::vector<Exhibit> ExhibitCommands::getAll() {
    auto repoExhibits = db.execute(
        [](db::DatabaseSession &session) { return repository::Exhibits{session}.getAll(); });

    std::sort(repoExhibits.begin(), repoExhibits.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name < rhs.name;
    });
    return std::vector<Exhibit>{repoExhibits.begin(), repoExhibits.end()};
}

NewExhibitsResponse ExhibitCommands::getNew(const NewExhibitsRequest &input) {
    auto repoExhibits = std::vector<repository::Exhibit>{};
    auto currentVersion = std::int32_t{};
    std::tie(currentVersion, repoExhibits) = db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto curVersion = countersRepo.get(repository::CounterType::LastExhibitVersion);

        auto exhibitsRepo = repository::Exhibits{session};
        auto exhibits = std::vector<repository::Exhibit>{};
        if (auto userVersion = input.acquiredVersion) {
            exhibits = exhibitsRepo.getAllNewerThan(userVersion.value());
        } else {
            exhibits = exhibitsRepo.getAll();
        }
        return std::make_tuple(curVersion, exhibits);
    });

    auto response = io::output::NewExhibitsResponse{};
    response.version = currentVersion;
    for (auto &repoExhibit : repoExhibits) {
        response.exhibits[repoExhibit.ID] = io::output::Exhibit{repoExhibit};
    }
    return response;
}

void ExhibitCommands::setFrame(const SetExhibitFrameRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.increment(repository::CounterType::LastExhibitVersion);

        auto exhibitsRepo = repository::Exhibits{session};
        auto exhibit = exhibitsRepo.getF(input.exhibitId);

        if (!exhibit.frame) {
            throw io::InvalidInput{"cannot change frame of exhibit without floor"};
        }

        auto repoFrame = exhibit.frame.value();
        repoFrame.x = input.frame.x;
        repoFrame.y = input.frame.y;
        repoFrame.width = input.frame.size.width;
        repoFrame.height = input.frame.size.height;

        exhibitsRepo.setVersion(exhibit.ID, version);
        exhibitsRepo.setFrame(exhibit.ID, repoFrame);
    });
}

Exhibit ExhibitCommands::update(const UpdateExhibitRequest &input) {
    auto repoExhibit = db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.increment(repository::CounterType::LastExhibitVersion);

        auto repo = repository::Exhibits{session};
        repo.setFrame(input.exhibitId, createFrame(input.floor, input.visibleFrame));
        repo.setVersion(input.exhibitId, version);
        return repo.getF(input.exhibitId);
    });
    return io::output::Exhibit{repoExhibit};
}

boost::optional<repository::Exhibit::Frame> ExhibitCommands::createFrame(
    const boost::optional<std::int32_t> &dstFloor,
    const boost::optional<io::MapFrame> &visibleFrame) const {
    static const std::int32_t screenProportion = 3;
    static const std::int32_t minSize = 200;

    if (!dstFloor) {
        return {};
    }

    auto frame = repository::Exhibit::Frame{};
    frame.floor = dstFloor.value();
    if (!visibleFrame || visibleFrame.value().floor != frame.floor) {
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