#include "CreateMultipleChoiceQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateMultipleChoiceQuestionRequest::CreateMultipleChoiceQuestionRequest(
    const communication::CreateMultipleChoiceQuestionRequest &thrift)
    : question(thrift.question), options(thrift.options), singleAnswer(thrift.singleAnswer) {
    if (thrift.__isset.name) {
        name = thrift.name;
    }
}
}
}
}