#include "CloneExperimentRequest.h"

namespace server {
namespace io {
namespace input {

CloneExperimentRequest::CloneExperimentRequest(const communication::CloneExperimentRequest &thrift)
    : ID(thrift.experimentId), name(thrift.name) {
}
}
}
}