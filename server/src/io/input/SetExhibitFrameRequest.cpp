#include "SetExhibitFrameRequest.h"

namespace io {
    namespace input {
        SetExhibitFrameRequest::SetExhibitFrameRequest(
            const communication::SetExhibitFrameRequest &thrift)
            : exhibitId(thrift.exhibitId),
              newX(thrift.x),
              newY(thrift.y),
              newWidth(thrift.width),
              newHeight(thrift.height) {
        }
    }
}