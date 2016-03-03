#ifndef SERVER_IO__MULTIPLE_CHOICE_QUESTION__H
#define SERVER_IO__MULTIPLE_CHOICE_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include <repository/MultipleChoiceQuestions.h>

#include "Question.h"

namespace server {
namespace io {

struct MultipleChoiceQuestion : public Question {
    struct Option {
        using thrift_t = communication::MultipleChoiceQuestionOption;
        using repo_t = repository::MultipleChoiceQuestion::Option;

        Option() = default;
        thrift_t toThrift() const;

        Option(const repo_t &repo);

        std::int32_t ID;
        std::string text;
    };

    using thrift_t = communication::MultipleChoiceQuestion;
    using repo_t = repository::MultipleChoiceQuestion;

    MultipleChoiceQuestion() = default;
    thrift_t toThrift() const;

    MultipleChoiceQuestion(const repo_t &repo);

    bool singleAnswer;
    std::vector<Option> options;
};
}
}

#endif