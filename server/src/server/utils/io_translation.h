#ifndef SERVER_UTILS__IO_TRANSLATION__H
#define SERVER_UTILS__IO_TRANSLATION__H

#include <db/struct/Action.h>
#include <db/struct/Exhibit.h>
#include <db/struct/MapElementFrame.h>
#include <db/struct/RawReport.h>

#include <server/io/Action.h>
#include <server/io/Exhibit.h>
#include <server/io/MapElementFrame.h>
#include <server/io/input/RawReport.h>

namespace server {
namespace utils {

io::Action toIO(const db::Action &action);
io::Exhibit toIO(const db::Exhibit &exhibit);
io::MapElementFrame toIO(const db::MapElementFrame &mapElementFrame);

db::RawReport toDB(const io::input::RawReport &report);
db::RawReport::Event toDB(const io::input::RawReport::Event &event);
}
}

#endif