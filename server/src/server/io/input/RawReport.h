#ifndef SERVER_IO_INPUT__RAW_REPORT__H
#define SERVER_IO_INPUT__RAW_REPORT__H

#include <cstdint>
#include <vector>
#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct RawReport {
    struct Event {
        Event(const communication::RawReportEvent &thrift);

        boost::optional<std::int32_t> exhibitId;
        std::int32_t durationInSecs;
        std::vector<std::int32_t> actions;
    };

    struct SimpleQuestionAnswer {
        SimpleQuestionAnswer(const communication::SimpleQuestionAnswer &thrift);

        boost::optional<std::string> answer;
    };

    RawReport(const communication::RawReport &thrift);

    std::int32_t experimentId;
    std::int32_t ID;

    std::vector<SimpleQuestionAnswer> simpleQuestionsAnswersBefore;
    std::vector<SimpleQuestionAnswer> simpleQuestionsAnswersAfter;

    std::vector<Event> history;
};
}
}
}

#endif