#include "CreateSimpleQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateSimpleQuestionRequest::CreateSimpleQuestionRequest(
    const communication::CreateSimpleQuestionRequest &thrift)
    : name(thrift.name),
      question(thrift.question),
      answerType(output::SimpleQuestion::AnswerTypeFromThrift(thrift.answerType)) {
}
}
}
}