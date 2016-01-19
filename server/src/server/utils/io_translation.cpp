#include "io_translation.h"

namespace server {
namespace utils {

io::Action toIO(const db::Action &action) {
    io::Action res;
    res.ID = action.ID;
    res.text = action.text;
    return res;
}

io::Exhibit toIO(const db::Exhibit &exhibit) {
    io::Exhibit res;
    res.name = exhibit.name;
    if (exhibit.frame) {
        res.frame = toIO(exhibit.frame.value());
    }
    return res;
}

io::MapElementFrame toIO(const db::MapElementFrame &mapElementFrame) {
    io::MapElementFrame res;
    res.x = mapElementFrame.x;
    res.y = mapElementFrame.y;
    res.width = mapElementFrame.width;
    res.height = mapElementFrame.height;
    res.mapLevel = mapElementFrame.mapLevel;
    return res;
}

db::RawReport toDB(const io::input::RawReport &report) {
    db::RawReport res;
    res.ID = report.reportId;
    for (const auto &e : report.history) {
        res.history.push_back(toDB(e));
    }
    return res;
}

db::RawReport::Event toDB(const io::input::RawReport::Event &event) {
    db::RawReport::Event res;
    res.exhibitId = event.exhibitId;
    res.durationInSecs = event.durationInSecs;
    res.actions = event.actions;
    return res;
}
}
}