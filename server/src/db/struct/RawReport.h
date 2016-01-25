#ifndef DB_STRUCT__RAW_REPORT__H
#define DB_STRUCT__RAW_REPORT__H

#include <cstdint>
#include <vector>
#include <string>

#include <boost/optional.hpp>

namespace db {

struct RawReport {
    struct Event {
        boost::optional<std::int32_t> exhibitId;
        std::int32_t durationInSecs;
        std::vector<std::int32_t> actions;
    };

    struct Survey {
        enum QuestionType : int {
            Simple = 0,
        };

        struct SimpleQuestion {
            boost::optional<std::string> answer;
        };

        std::vector<QuestionType> order;
        std::vector<SimpleQuestion> simpleQuestions;
    };

    std::int32_t ID;
    std::int32_t experimentID;
    std::vector<Event> history;

    Survey surveyBefore;
    Survey surveyAfter;
};
}

#endif