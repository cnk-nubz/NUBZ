#include "SetMapImageRequest.h"

namespace server {
namespace io {
namespace input {

SetMapImageRequest::SetMapImageRequest(const communication::SetMapImageRequest &thrift) {
    floor = thrift.floor;
    filename = thrift.filename;
}
}
}
}