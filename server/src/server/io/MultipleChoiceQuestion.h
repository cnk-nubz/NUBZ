#ifndef SERVER_IO__MULTIPLE_CHOICE_QUESTION__H
#define SERVER_IO__MULTIPLE_CHOICE_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include "Question.h"

namespace server {
namespace io {

struct MultipleChoiceQuestion : public Question {
    struct Option {
        using thrift_t = communication::MultipleChoiceQuestionOption;

        Option() = default;
        thrift_t toThrift() const;

        std::int32_t ID;
        std::string text;
    };

    using thrift_t = communication::MultipleChoiceQuestion;

    MultipleChoiceQuestion() = default;
    thrift_t toThrift() const;

    bool singleAnswer;
    std::vector<Option> options;
};
}
}

#endif