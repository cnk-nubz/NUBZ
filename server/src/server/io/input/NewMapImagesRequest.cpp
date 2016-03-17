#include "NewMapImagesRequest.h"

namespace server {
namespace io {
namespace input {

NewMapImagesRequest::NewMapImagesRequest(const communication::NewMapImagesRequest &thrift) {
    if (thrift.__isset.acquiredVersion) {
        acquiredVersion = thrift.acquiredVersion;
    }
}
}
}
}