#include "CreateSortQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateSortQuestionRequest::CreateSortQuestionRequest(
    const communication::CreateSortQuestionRequest &thrift)
    : name(thrift.name), question(thrift.question), options(thrift.options) {
}
}
}
}