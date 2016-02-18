#include "CreateSimpleQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateSimpleQuestionRequest::CreateSimpleQuestionRequest(
    const communication::CreateSimpleQuestionRequest &thrift)
    : question(thrift.question),
      answerType(SimpleQuestion::AnswerTypeFromThrift(thrift.answerType)) {
    if (thrift.__isset.name) {
        name = thrift.name;
    }
}
}
}
}