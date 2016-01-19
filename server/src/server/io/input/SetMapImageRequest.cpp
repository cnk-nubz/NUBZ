#include "SetMapImageRequest.h"

namespace server {
namespace io {
namespace input {

SetMapImageRequest::SetMapImageRequest(const communication::SetMapImageRequest &thrift) {
    level = thrift.level;
    filename = thrift.filename;
}
}
}
}