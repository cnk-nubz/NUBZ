#include "UpdateExhibitRequest.h"

namespace server {
namespace io {
namespace input {

UpdateExhibitRequest::UpdateExhibitRequest(const communication::UpdateExhibitRequest &thrift)
    : exhibitId(thrift.exhibitId),
      rgbHex(thrift.rgbHex) {
    if (thrift.__isset.visibleFrame) {
        visibleFrame = MapFrame{thrift.visibleFrame};
    }
    if (thrift.__isset.floor) {
        floor = thrift.floor;
    }
}
}
}
}
