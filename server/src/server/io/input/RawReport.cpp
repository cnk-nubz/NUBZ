#include "RawReport.h"

namespace server {
namespace io {
namespace input {

RawReport::RawReport(const communication::RawReport &thrift)
    : experimentId(thrift.experimentId), ID(thrift.reportId) {
    for (const auto &q : thrift.simpleQuestionsAnswersBefore) {
        simpleQuestionsAnswersBefore.emplace_back(q);
    }
    for (const auto &q : thrift.simpleQuestionsAnswersAfter) {
        simpleQuestionsAnswersAfter.emplace_back(q);
    }
    for (const auto &event : thrift.history) {
        history.emplace_back(event);
    }
}

RawReport::SimpleQuestionAnswer::SimpleQuestionAnswer(
    const communication::SimpleQuestionAnswer &thrift) {
    if (thrift.__isset.answer) {
        answer = thrift.answer;
    }
}

RawReport::Event::Event(const communication::RawReportEvent &thrift)
    : actions(thrift.actions), durationInSecs(thrift.durationInSecs) {
    if (thrift.__isset.exhibitId) {
        exhibitId = thrift.exhibitId;
    }
}
}
}
}