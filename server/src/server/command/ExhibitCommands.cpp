#include <algorithm>

#include <server/io/InvalidInput.h>
#include <server/utils/CmpUTF8.h>

#include "ExhibitCommands.h"

namespace server {
namespace command {

ExhibitCommands::ExhibitCommands(db::Database &db) : db(db) {
}

Exhibit ExhibitCommands::create(const CreateExhibitRequest &input) {
    auto exhibit = repository::Exhibit{};
    exhibit.name = input.name;
    exhibit.rgbHex = input.rgbHex;
    exhibit.frame = createFrame(input.floor, input.visibleFrame);

    db.execute([&](db::DatabaseSession &session) {
        auto exhibitsRepo = repository::Exhibits{session};
        exhibitsRepo.insert(&exhibit);
    });

    return Exhibit{exhibit};
}

std::vector<Exhibit> ExhibitCommands::getAll() {
    auto repoExhibits = db.execute(
        [](db::DatabaseSession &session) { return repository::Exhibits{session}.getAll(); });

    std::sort(repoExhibits.begin(), repoExhibits.end(), [](const auto &lhs, const auto &rhs) {
        return utils::cmpUTF8(lhs.name, rhs.name);
    });
    return std::vector<Exhibit>{repoExhibits.begin(), repoExhibits.end()};
}

NewExhibitsResponse ExhibitCommands::getNew(const NewExhibitsRequest &input) {
    auto response = NewExhibitsResponse{};
    auto exhibits = std::vector<repository::Exhibit>{};

    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Exhibits{session};
        if (!input.acquiredVersion ||
            input.acquiredVersion.value() < repo.getLastDeletedVersion()) {
            exhibits = repo.getAll();
            response.fullRefresh = true;
        } else {
            exhibits = repo.getAllNewerThan(input.acquiredVersion.value());
            response.fullRefresh = false;
        }
        response.version = repo.getCurrentVersion();
    });

    for (auto &e : exhibits) {
        response.exhibits[e.ID] = Exhibit{e};
    }
    return response;
}

void ExhibitCommands::setFrame(const SetExhibitFrameRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto exhibitsRepo = repository::Exhibits{session};
        auto exhibit = exhibitsRepo.get(input.exhibitId);

        if (!exhibit.frame) {
            throw io::InvalidInput{"cannot change frame of exhibit without floor"};
        }

        auto repoFrame = exhibit.frame.value();
        repoFrame.x = input.frame.x;
        repoFrame.y = input.frame.y;
        repoFrame.width = input.frame.size.width;
        repoFrame.height = input.frame.size.height;
        // floor stays the same
        exhibitsRepo.setFrame(exhibit.ID, repoFrame);
    });
}

void ExhibitCommands::update(const UpdateExhibitRequest &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Exhibits{session};
        auto oldExhibit = repo.get(input.exhibitId);

        repo.setRgbHex(input.exhibitId, input.rgbHex);
        if (!input.floor || !oldExhibit.frame ||
            input.floor.value() != oldExhibit.frame.value().floor) {
            repo.setFrame(input.exhibitId, createFrame(input.floor, input.visibleFrame));
        }
    });
}

void ExhibitCommands::remove(std::int32_t exhibitID) {
    db.execute(
        [&](db::DatabaseSession &session) { repository::Exhibits{session}.remove(exhibitID); });
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
