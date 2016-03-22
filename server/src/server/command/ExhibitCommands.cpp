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
    if (!server::utils::checkText(input.name)) {
        throw io::InvalidInput{"name contains invalid characters"};
    }

    auto exhibit = repository::Exhibit{};
    exhibit.name = input.name;
    exhibit.rgbHex = input.rgbHex;
    exhibit.frame = createFrame(input.floor, input.visibleFrame);

    db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.increment(repository::CounterType::LastExhibitVersion);

        exhibit.version = version;
        auto exhibitsRepo = repository::Exhibits{session};
        exhibitsRepo.insert(&exhibit);
    });

    return Exhibit{exhibit};
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
    auto exhibits = std::vector<repository::Exhibit>{};
    auto currentVersion = std::int32_t{};
    std::tie(currentVersion, exhibits) = db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto version = countersRepo.get(repository::CounterType::LastExhibitVersion);

        auto exhibitsRepo = repository::Exhibits{session};
        if (auto userVersion = input.acquiredVersion) {
            return std::make_tuple(version, exhibitsRepo.getAllNewerThan(userVersion.value()));
        } else {
            return std::make_tuple(version, exhibitsRepo.getAll());
        }
    });

    auto response = NewExhibitsResponse{};
    response.version = currentVersion;
    for (auto &e : exhibits) {
        response.exhibits[e.ID] = Exhibit{e};
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
        auto oldExhibit = repo.getF(input.exhibitId);

        repo.setVersion(input.exhibitId, version);
        repo.setRgbHex(input.exhibitId, input.rgbHex);
        if (!input.floor || !oldExhibit.frame ||
            input.floor.value() != oldExhibit.frame.value().floor) {
            repo.setFrame(input.exhibitId, createFrame(input.floor, input.visibleFrame));
        }

        return repo.getF(input.exhibitId);
    });
    return Exhibit{repoExhibit};
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
        frame.x = std::max<std::int32_t>(0, middleX - frame.width / 2);
        frame.y = std::max<std::int32_t>(0, middleY - frame.height / 2);
    }

    return frame;
}
}
}
