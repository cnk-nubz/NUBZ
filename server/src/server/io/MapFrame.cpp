#include "MapFrame.h"

namespace server {
namespace io {

MapFrame::MapFrame(const communication::MapFrame &thrift)
    : frame(thrift.frame), floor(thrift.floor) {
}

communication::MapFrame MapFrame::toThrift() const {
    auto res = communication::MapFrame{};
    res.frame = frame.toThrift();
    res.floor = floor;
    return res;
}
}
}