#include "NewExhibitRequest.h"

namespace io {
    namespace input {
        NewExhibitRequest::NewExhibitRequest(const communication::NewExhibitRequest &thrift)
            : name(thrift.name) {
            if (thrift.__isset.floor) {
                floor = thrift.floor;
            }
            if (thrift.__isset.visibleMapFrame) {
                visibleMapFrame = io::MapElementFrame(thrift.visibleMapFrame);
            }
        }
    }
}