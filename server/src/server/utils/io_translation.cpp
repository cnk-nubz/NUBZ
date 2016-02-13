#include <utils/fp_algorithm.h>

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
    res.ID = exhibit.ID;
    if (exhibit.frame) {
        res.mapFrame = toIO(exhibit.frame.value());
    }
    return res;
}

io::MapFrame toIO(const db::MapFrame &mapFrame) {
    io::MapFrame res;
    res.frame.x = mapFrame.x;
    res.frame.y = mapFrame.y;
    res.frame.size.width = mapFrame.width;
    res.frame.size.height = mapFrame.height;
    res.floor = mapFrame.floor;
    return res;
}

io::SimpleQuestion toIO(const db::SimpleQuestion &simpleQuestion) {
    io::SimpleQuestion res;
    res.ID = simpleQuestion.ID;
    res.name = simpleQuestion.name;
    res.question = simpleQuestion.question;
    res.answerType = (simpleQuestion.numberAnswer) ? io::SimpleQuestion::AnswerType::Number
                                                   : io::SimpleQuestion::AnswerType::Text;
    return res;
}

db::RawReport toDB(const io::input::RawReport &report) {
    db::RawReport res;
    res.ID = report.ID;
    ::utils::transform(report.history, res.history, [](const auto &e) { return toDB(e); });
    ::utils::transform(report.answersBefore.simpleQuestionsAnswers,
                       res.surveyBefore.simpleQuestions,
                       [](const auto &e) { return toDB(e); });
    ::utils::transform(report.answersAfter.simpleQuestionsAnswers,
                       res.surveyAfter.simpleQuestions,
                       [](const auto &e) { return toDB(e); });
    return res;
}

db::RawReport::Event toDB(const io::input::RawReport::Event &event) {
    db::RawReport::Event res;
    res.exhibitId = event.exhibitId;
    res.durationInSecs = event.durationInSecs;
    res.actions = event.actions;
    return res;
}

db::RawReport::Survey::SimpleQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::SimpleQuestionAnswer &answer) {
    db::RawReport::Survey::SimpleQuestionAnswer res;
    res.answer = answer.answer;
    return res;
}
}
}