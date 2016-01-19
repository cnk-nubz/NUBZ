#include "ExhibitsRequest.h"

namespace server {
namespace io {
namespace input {

ExhibitsRequest::ExhibitsRequest(const communication::ExhibitsRequest &thrift) {
    if (thrift.__isset.acquiredVersion) {
        acquiredVersion = thrift.acquiredVersion;
    }
}
}
}
}