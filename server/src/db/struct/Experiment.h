#ifndef DB_STRUCT__EXPERIMENT__H
#define DB_STRUCT__EXPERIMENT__H

#include <cstdint>
#include <string>
#include <vector>

namespace db {

struct Experiment {
    struct Survey {
        enum QuestionType : int {
            Simple = 0,
            MultipleChoice = 1,
            Sort = 2,
        };

        std::vector<QuestionType> order;
        std::vector<std::int32_t> simpleQuestions;
        std::vector<std::int32_t> multipleChoiceQuestions;
        std::vector<std::int32_t> sortQuestions;
    };

    std::int32_t ID;
    std::string name;
    std::vector<std::int32_t> actions;
    std::vector<std::int32_t> breakActions;
    Survey surveyBefore;
    Survey surveyAfter;
};
}

#endif