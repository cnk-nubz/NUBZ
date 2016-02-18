#include "CreateMultipleChoiceQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateMultipleChoiceQuestionRequest::CreateMultipleChoiceQuestionRequest(
    const communication::CreateMultipleChoiceQuestionRequest &thrift)
    : question(thrift.question), singleAnswer(thrift.singleAnswer), options(thrift.options) {
    if (thrift.__isset.name) {
        name = thrift.name;
    }
}
}
}
}