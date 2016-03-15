#include "SimpleQuestion.h"

namespace server {
namespace io {
namespace output {

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
    auto res = communication::SimpleQuestion{};
    res.questionId = ID;
    res.name = name;
    res.question = question;
    res.answerType = AnswerTypeToThrift(answerType);
    return res;
}

SimpleQuestion::SimpleQuestion(const repository::SimpleQuestion &repo) {
    ID = repo.ID;
    name = repo.name;
    question = repo.question;
    answerType = repo.numberAnswer ? AnswerType::Number : AnswerType::Text;
}
}
}
}