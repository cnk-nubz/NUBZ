#include "CreateExhibitRequest.h"

namespace server {
namespace io {
namespace input {

CreateExhibitRequest::CreateExhibitRequest(const communication::CreateExhibitRequest &thrift)
    : name(thrift.name) {
    if (thrift.__isset.floor) {
        floor = thrift.floor;
    }
    if (thrift.__isset.visibleFrame) {
        visibleFrame = io::MapFrame(thrift.visibleFrame);
    }
}
}
}
}