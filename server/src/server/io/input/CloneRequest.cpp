#include "CloneRequest.h"

namespace server {
namespace io {
namespace input {

CloneRequest::CloneRequest(const communication::CloneRequest &thrift)
    : ID(thrift.id), name(thrift.name) {
}
}
}
}