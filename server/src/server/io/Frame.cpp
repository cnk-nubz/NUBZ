#include "Frame.h"

namespace server {
namespace io {

Frame::Frame(const communication::Frame &thrift) : x(thrift.x), y(thrift.y), size(thrift.size) {
}

communication::Frame Frame::toThrift() const {
    communication::Frame thrift;
    thrift.x = x;
    thrift.y = y;
    thrift.size = size.toThrift();
    return thrift;
}
}
}