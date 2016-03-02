#ifndef SERVER_UTILS__IO_TRANSLATION__H
#define SERVER_UTILS__IO_TRANSLATION__H

#include <vector>

#include <db/struct/Exhibit.h>
#include <db/struct/MapFrame.h>
#include <db/struct/MultipleChoiceQuestion.h>
#include <db/struct/MultipleChoiceQuestionOption.h>
#include <db/struct/RawReport.h>

#include <server/io/Exhibit.h>
#include <server/io/MapFrame.h>
#include <server/io/MultipleChoiceQuestion.h>
#include <server/io/SimpleQuestion.h>
#include <server/io/SortQuestion.h>
#include <server/io/input/RawReport.h>

namespace server {
namespace utils {

io::Exhibit toIO(const db::Exhibit &exhibit);
io::MapFrame toIO(const db::MapFrame &mapFrame);
io::MultipleChoiceQuestion toIO(const db::MultipleChoiceQuestion &question,
                                const std::vector<db::MultipleChoiceQuestionOption> &options);
io::MultipleChoiceQuestion::Option toIO(const db::MultipleChoiceQuestionOption &option);

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