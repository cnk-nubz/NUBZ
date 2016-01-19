#include "MapElementFrame.h"

namespace server {
namespace io {

MapElementFrame::MapElementFrame(const communication::MapElementFrame &thrift)
    : x(thrift.x),
      y(thrift.y),
      width(thrift.width),
      height(thrift.height),
      mapLevel(thrift.mapLevel) {
}

communication::MapElementFrame MapElementFrame::toThrift() const {
    communication::MapElementFrame thrift;
    thrift.x = x;
    thrift.y = y;
    thrift.width = width;
    thrift.height = height;
    thrift.mapLevel = mapLevel;
    return thrift;
}
}
}