#include "utils.h"
#include "MultipleChoiceQuestion.h"

namespace server {
namespace io {

communication::MultipleChoiceQuestion MultipleChoiceQuestion::toThrift() const {
    communication::MultipleChoiceQuestion thrift;
    thrift.questionId = ID;
    thrift.name = name;
    thrift.question = question;
    thrift.singleAnswer = singleAnswer;
    thrift.options = ioToThrift(options);
    return thrift;
}

communication::MultipleChoiceQuestionOption MultipleChoiceQuestion::Option::toThrift() const {
    communication::MultipleChoiceQuestionOption thrift;
    thrift.optionId = ID;
    thrift.text = text;
    return thrift;
}
}
}