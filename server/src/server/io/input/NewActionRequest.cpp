#include "NewActionRequest.h"

namespace server {
namespace io {
namespace input {

NewActionRequest::NewActionRequest(const communication::NewActionRequest &thrift)
    : text(thrift.text) {
}
}
}
}