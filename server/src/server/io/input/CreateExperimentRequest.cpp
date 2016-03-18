#include "CreateExperimentRequest.h"

namespace server {
namespace io {
namespace input {

CreateExperimentRequest::CreateExperimentRequest(
    const communication::CreateExperimentRequest &thrift)
    : name(thrift.name),
      surveyBefore(thrift.surveyBefore),
      actions(thrift.exhibitActions),
      breakActions(thrift.breakActions),
      surveyAfter(thrift.surveyAfter) {
}
}
}
}