#include "SetExhibitFrameRequest.h"

namespace server {
namespace io {
namespace input {

SetExhibitFrameRequest::SetExhibitFrameRequest(const communication::SetExhibitFrameRequest &thrift)
    : exhibitId(thrift.exhibitId), frame(thrift.frame) {
}
}
}
}