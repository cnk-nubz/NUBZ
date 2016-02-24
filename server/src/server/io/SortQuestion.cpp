#include "utils.h"
#include "SortQuestion.h"

namespace server {
namespace io {

communication::SortQuestion SortQuestion::toThrift() const {
    communication::SortQuestion thrift;
    thrift.questionId = ID;
    thrift.name = name;
    thrift.question = question;
    thrift.options = ioToThrift(options);
    return thrift;
}

communication::SortQuestionOption SortQuestion::Option::toThrift() const {
    communication::SortQuestionOption thrift;
    thrift.optionId = ID;
    thrift.text = text;
    return thrift;
}
}
}