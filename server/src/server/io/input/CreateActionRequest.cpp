#include "CreateActionRequest.h"

namespace server {
namespace io {
namespace input {

CreateActionRequest::CreateActionRequest(const communication::CreateActionRequest &thrift)
    : text(thrift.text) {
}
}
}
}