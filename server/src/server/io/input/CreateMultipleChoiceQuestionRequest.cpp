#include "CreateMultipleChoiceQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateMultipleChoiceQuestionRequest::CreateMultipleChoiceQuestionRequest(
    const communication::CreateMultipleChoiceQuestionRequest &thrift)
    : name(thrift.name),
      question(thrift.question),
      options(thrift.options),
      singleAnswer(thrift.singleAnswer) {
}
}
}
}