#ifndef SERVER_IO_INPUT__RAW_REPORT__H
#define SERVER_IO_INPUT__RAW_REPORT__H

#include <cstdint>
#include <string>
#include <vector>

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

    struct SurveyAnswers {
        struct SimpleQuestionAnswer {
            SimpleQuestionAnswer(const communication::SimpleQuestionAnswer &thrift);

            boost::optional<std::string> answer;
        };

        struct MultipleChoiceQuestionAnswer {
            MultipleChoiceQuestionAnswer(const communication::MultipleChoiceQuestionAnswer &thrift);

            boost::optional<std::vector<std::int32_t>> choosenOptions;
        };

        struct SortQuestionAnswer {
            SortQuestionAnswer(const communication::SortQuestionAnswer &thrift);

            boost::optional<std::vector<std::int32_t>> choosenOrder;
        };

        SurveyAnswers(const communication::SurveyAnswers &thrift);

        std::vector<SimpleQuestionAnswer> simpleQuestionsAnswers;
        std::vector<MultipleChoiceQuestionAnswer> multipleChoiceQuestionsAnswers;
        std::vector<SortQuestionAnswer> sortQuestionsAnswers;
    };

    RawReport(const communication::RawReport &thrift);

    std::int32_t experimentId;
    std::int32_t ID;

    SurveyAnswers answersBefore;
    SurveyAnswers answersAfter;

    std::vector<Event> history;
};
}
}
}

#endif