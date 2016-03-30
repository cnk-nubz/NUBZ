#include "RawReport.h"

namespace server {
namespace io {
namespace input {

RawReport::RawReport(const communication::RawReport &thrift)
    : experimentId(thrift.experimentId),
      ID(thrift.reportId),
      answersBefore(thrift.answersBefore),
      answersAfter(thrift.answersAfter) {
    for (const auto &event : thrift.history) {
        history.emplace_back(event);
    }
}

RawReport::Event::Event(const communication::RawReportEvent &thrift)
    : durationInSecs(thrift.durationInSecs), actions(thrift.actions) {
    if (thrift.__isset.exhibitId) {
        exhibitId = thrift.exhibitId;
    }
}

RawReport::SurveyAnswers::SurveyAnswers(const communication::SurveyAnswers &thrift) {
    for (const auto &raw : thrift.simpleQuestionsAnswers) {
        simpleQuestionsAnswers.push_back(raw.answer);
    }
    for (const auto &raw : thrift.multipleChoiceQuestionsAnswers) {
        multipleChoiceQuestionsAnswers.push_back(raw.choosenOptions);
    }
    for (const auto &raw : thrift.sortQuestionsAnswers) {
        sortQuestionsAnswers.push_back(raw.choosenOrder);
    }
}

repository::Report RawReport::toRepo() const {
    auto res = repository::Report{};
    res.ID = ID;
    res.experimentID = experimentId;
    for (auto &event : history) {
        res.history.push_back(event.toRepo());
    }
    res.surveyBefore = answersBefore.toRepo();
    res.surveyAfter = answersAfter.toRepo();
    return res;
}

repository::Report::Event RawReport::Event::toRepo() const {
    auto res = repository::Report::Event{};
    res.exhibitID = exhibitId;
    res.durationInSecs = durationInSecs;
    res.actions = actions;
    return res;
}

repository::Report::SurveyAns RawReport::SurveyAnswers::toRepo() const {
    auto res = repository::Report::SurveyAns{};
    res.simpleQAnswers = simpleQuestionsAnswers;
    res.multiChoiceQAnswers = multipleChoiceQuestionsAnswers;
    res.sortQAnswers = sortQuestionsAnswers;
    return res;
}
}
}
}