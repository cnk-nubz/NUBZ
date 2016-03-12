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
        auto mapFrame = MapFrame{};
        mapFrame.frame.x = repoFrame.x;
        mapFrame.frame.y = repoFrame.y;
        mapFrame.frame.size.width = repoFrame.width;
        mapFrame.frame.size.height = repoFrame.height;
        mapFrame.floor = repoFrame.floor;
        this->mapFrame = mapFrame;
    }
}
}
}
}