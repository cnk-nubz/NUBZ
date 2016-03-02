#ifndef SERVER_IO__SIMPLE_QUESTION__H
#define SERVER_IO__SIMPLE_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include <repository/SimpleQuestions.h>

#include "Question.h"

namespace server {
namespace io {

struct SimpleQuestion : public Question {
    using thrift_t = communication::SimpleQuestion;
    using repo_t = repository::SimpleQuestion;

    enum class AnswerType {
        Text,
        Number,
    };

    SimpleQuestion() = default;
    thrift_t toThrift() const;

    SimpleQuestion(const repo_t &repo);

    static AnswerType AnswerTypeFromThrift(
        const communication::SimpleQuestionAnswerType::type &thrift);
    static communication::SimpleQuestionAnswerType::type AnswerTypeToThrift(const AnswerType &type);

    AnswerType answerType;
};
}
}

#endif