#include "CreateSortQuestionRequest.h"

namespace server {
namespace io {
namespace input {

CreateSortQuestionRequest::CreateSortQuestionRequest(
    const communication::CreateSortQuestionRequest &thrift)
    : question(thrift.question), options(thrift.options) {
    if (thrift.__isset.name) {
        name = thrift.name;
    }
}
}
}
}