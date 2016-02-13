#include "MapFrame.h"

namespace server {
namespace io {

MapFrame::MapFrame(const communication::MapFrame &thrift)
    : frame(thrift.frame), floor(thrift.floor) {
}

communication::MapFrame MapFrame::toThrift() const {
    communication::MapFrame thrift;
    thrift.frame = frame.toThrift();
    thrift.floor = floor;
    return thrift;
}
}
}