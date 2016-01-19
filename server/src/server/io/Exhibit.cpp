#include "Exhibit.h"

namespace server {
namespace io {

Exhibit::Exhibit(const communication::Exhibit &thrift) : name(thrift.name) {
    if (thrift.__isset.frame) {
        frame = thrift.frame;
    }
}

communication::Exhibit Exhibit::toThrift() const {
    communication::Exhibit thrift;
    thrift.name = name;
    if (frame) {
        thrift.__set_frame(frame.value().toThrift());
    }
    return thrift;
}
}
}