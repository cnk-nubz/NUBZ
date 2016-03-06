#ifndef SERVER_UTILS__IO_TRANSLATION__H
#define SERVER_UTILS__IO_TRANSLATION__H

#include <vector>

#include <db/struct/RawReport.h>

#include <server/io/MapFrame.h>
#include <server/io/input/RawReport.h>
#include <server/io/output/MultipleChoiceQuestion.h>

namespace server {
namespace utils {

db::RawReport toDB(const io::input::RawReport &report);
db::RawReport::Event toDB(const io::input::RawReport::Event &event);
db::RawReport::Survey::SimpleQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::SimpleQuestionAnswer &answer);
db::RawReport::Survey::MultipleChoiceQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::MultipleChoiceQuestionAnswer &answer);
db::RawReport::Survey::SortQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::SortQuestionAnswer &answer);
}
}

#endif