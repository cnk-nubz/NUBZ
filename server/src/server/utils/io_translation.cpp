#include <utils/fp_algorithm.h>

#include "io_translation.h"

namespace server {
namespace utils {

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
    auto res = db::RawReport::Survey::MultipleChoiceQuestionAnswer{};
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