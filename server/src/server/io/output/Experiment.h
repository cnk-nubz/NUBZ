#ifndef SERVER_IO_OUTPUT__EXPERIMENT__H
#define SERVER_IO_OUTPUT__EXPERIMENT__H

#include <cstdint>
#include <string>
#include <vector>

#include <communication/structs_types.h>

#include <server/io/SimpleQuestion.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

struct Experiment {
    struct Survey {
        enum class QuestionType {
            Simple,
        };
        static QuestionType QuestionTypeFromThrift(const communication::QuestionType::type &thrift);
        static communication::QuestionType::type QuestionTypeToThrift(const QuestionType &type);

        using thrift_t = communication::Survey;

        Survey() = default;
        thrift_t toThrift() const;

        std::vector<QuestionType> questionsOrder;
        std::vector<SimpleQuestion> simpleQuestions;
    };

    using thrift_t = communication::Experiment;

    Experiment() = default;
    thrift_t toThrift() const;

    std::int32_t ID;
    std::string name;

    std::vector<Action> exhibitActions;
    std::vector<Action> breakActions;

    Survey surveyBefore;
    Survey surveyAfter;
};
}
}
}

#endif