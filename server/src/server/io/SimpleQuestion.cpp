#include "SimpleQuestion.h"

namespace server {
namespace io {

SimpleQuestion::AnswerType SimpleQuestion::AnswerTypeFromThrift(
    const communication::SimpleQuestionAnswerType::type &thrift) {
    switch (thrift) {
        case communication::SimpleQuestionAnswerType::type::TEXT:
            return SimpleQuestion::AnswerType::Text;
        case communication::SimpleQuestionAnswerType::type::NUMBER:
            return SimpleQuestion::AnswerType::Number;
    }
}

communication::SimpleQuestionAnswerType::type SimpleQuestion::AnswerTypeToThrift(
    const SimpleQuestion::AnswerType &type) {
    switch (type) {
        case SimpleQuestion::AnswerType::Text:
            return communication::SimpleQuestionAnswerType::type::TEXT;
        case SimpleQuestion::AnswerType::Number:
            return communication::SimpleQuestionAnswerType::type::NUMBER;
    }
}

communication::SimpleQuestion SimpleQuestion::toThrift() const {
    communication::SimpleQuestion thrift;
    thrift.questionId = ID;
    thrift.name = name;
    thrift.question = question;
    thrift.answerType = AnswerTypeToThrift(answerType);
    return thrift;
}
}
}