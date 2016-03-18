#include "Frame.h"

namespace server {
namespace io {

Frame::Frame(const communication::Frame &thrift) : x(thrift.x), y(thrift.y), size(thrift.size) {
}

communication::Frame Frame::toThrift() const {
    auto res = communication::Frame{};
    res.x = x;
    res.y = y;
    res.size = size.toThrift();
    return res;
}
}
}