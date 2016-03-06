#include "Exhibit.h"

namespace server {
namespace io {
namespace output {

communication::Exhibit Exhibit::toThrift() const {
    auto res = communication::Exhibit{};
    res.exhibitId = ID;
    res.name = name;
    if (mapFrame) {
        res.__set_mapFrame(mapFrame.value().toThrift());
    }
    return res;
}

Exhibit::Exhibit(const repository::Exhibit &repo) : ID(repo.ID), name(repo.name) {
    if (repo.frame) {
        auto repoFrame = repo.frame.value();
        auto frame = MapFrame{};
        frame.frame.x = repoFrame.x;
        frame.frame.y = repoFrame.y;
        frame.frame.size.width = repoFrame.width;
        frame.frame.size.height = repoFrame.height;
        frame.floor = repoFrame.floor;
        mapFrame = frame;
    }
}
}
}
}