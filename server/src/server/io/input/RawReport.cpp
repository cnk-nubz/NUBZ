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
    : actions(thrift.actions), durationInSecs(thrift.durationInSecs) {
    if (thrift.__isset.exhibitId) {
        exhibitId = thrift.exhibitId;
    }
}

RawReport::SurveyAnswers::SurveyAnswers(const communication::SurveyAnswers &thrift) {
    for (const auto &raw : thrift.simpleQuestionsAnswers) {
        simpleQuestionsAnswers.emplace_back(raw);
    }
}

RawReport::SurveyAnswers::SimpleQuestionAnswer::SimpleQuestionAnswer(
    const communication::SimpleQuestionAnswer &thrift) {
    if (thrift.__isset.answer) {
        answer = thrift.answer;
    }
}
}
}
}