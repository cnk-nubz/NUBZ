#include <utils/fp_algorithm.h>

#include "io_translation.h"

namespace server {
namespace utils {

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

io::MultipleChoiceQuestion toIO(const db::MultipleChoiceQuestion &question,
                                const std::vector<db::MultipleChoiceQuestionOption> &options) {
    io::MultipleChoiceQuestion res;
    res.ID = question.ID;
    res.name = question.name;
    res.question = question.question;
    res.singleAnswer = question.singleAnswer;
    ::utils::transform(options, res.options, [](const auto &o) { return toIO(o); });
    return res;
}

io::MultipleChoiceQuestion::Option toIO(const db::MultipleChoiceQuestionOption &questionOption) {
    io::MultipleChoiceQuestion::Option res;
    res.ID = questionOption.ID;
    res.text = questionOption.text;
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

    ::utils::transform(report.answersBefore.multipleChoiceQuestionsAnswers,
                       res.surveyBefore.multipleChoiceQuestions,
                       [](const auto &e) { return toDB(e); });
    ::utils::transform(report.answersAfter.multipleChoiceQuestionsAnswers,
                       res.surveyAfter.multipleChoiceQuestions,
                       [](const auto &e) { return toDB(e); });

    ::utils::transform(report.answersBefore.sortQuestionsAnswers,
                       res.surveyBefore.sortQuestions,
                       [](const auto &e) { return toDB(e); });
    ::utils::transform(report.answersAfter.sortQuestionsAnswers,
                       res.surveyAfter.sortQuestions,
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

db::RawReport::Survey::MultipleChoiceQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::MultipleChoiceQuestionAnswer &answer) {
    db::RawReport::Survey::MultipleChoiceQuestionAnswer res;
    res.choosenOptions = answer.choosenOptions;
    return res;
}

db::RawReport::Survey::SortQuestionAnswer toDB(
    const io::input::RawReport::SurveyAnswers::SortQuestionAnswer &answer) {
    db::RawReport::Survey::SortQuestionAnswer res;
    res.choosenOrder = answer.choosenOrder;
    return res;
}
}
}