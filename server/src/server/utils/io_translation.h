#ifndef SERVER_UTILS__IO_TRANSLATION__H
#define SERVER_UTILS__IO_TRANSLATION__H

#include <db/struct/Action.h>
#include <db/struct/Exhibit.h>
#include <db/struct/MapFrame.h>
#include <db/struct/RawReport.h>
#include <db/struct/SimpleQuestion.h>

#include <server/io/Action.h>
#include <server/io/Exhibit.h>
#include <server/io/MapFrame.h>
#include <server/io/SimpleQuestion.h>
#include <server/io/input/RawReport.h>

namespace server {
namespace utils {

io::Action toIO(const db::Action &action);
io::Exhibit toIO(const db::Exhibit &exhibit);
io::MapFrame toIO(const db::MapFrame &mapFrame);
io::SimpleQuestion toIO(const db::SimpleQuestion &simpleQuestion);

db::RawReport toDB(const io::input::RawReport &report);
db::RawReport::Event toDB(const io::input::RawReport::Event &event);
db::RawReport::Survey::SimpleQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::SimpleQuestionAnswer &answer);
}
}

#endif