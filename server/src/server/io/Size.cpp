#include "Size.h"

namespace server {
namespace io {
Size::Size(const communication::Size &thrift) : width(thrift.width), height(thrift.height) {
}

communication::Size Size::toThrift() const {
    communication::Size thrift;
    thrift.width = width;
    thrift.height = height;
    return thrift;
}
}
}