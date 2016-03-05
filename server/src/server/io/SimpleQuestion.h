#ifndef SERVER_IO__SIMPLE_QUESTION__H
#define SERVER_IO__SIMPLE_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include "Question.h"

namespace server {
namespace io {

struct SimpleQuestion : public Question {
    enum class AnswerType {
        Text,
        Number,
    };
    static AnswerType AnswerTypeFromThrift(
        const communication::SimpleQuestionAnswerType::type &thrift);
    static communication::SimpleQuestionAnswerType::type AnswerTypeToThrift(const AnswerType &type);

    using thrift_t = communication::SimpleQuestion;

    SimpleQuestion() = default;
    thrift_t toThrift() const;

    AnswerType answerType;
};
}
}

#endif