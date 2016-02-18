#include "NewExhibitsRequest.h"

namespace server {
namespace io {
namespace input {

NewExhibitsRequest::NewExhibitsRequest(const communication::NewExhibitsRequest &thrift) {
    if (thrift.__isset.acquiredVersion) {
        acquiredVersion = thrift.acquiredVersion;
    }
}
}
}
}