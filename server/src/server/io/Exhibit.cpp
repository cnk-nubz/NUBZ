#include "Exhibit.h"

namespace server {
namespace io {

Exhibit::Exhibit(const communication::Exhibit &thrift) : ID(thrift.exhibitId), name(thrift.name) {
    if (thrift.__isset.mapFrame) {
        mapFrame = MapFrame(thrift.mapFrame);
    }
}

communication::Exhibit Exhibit::toThrift() const {
    communication::Exhibit thrift;
    thrift.exhibitId = ID;
    thrift.name = name;
    if (mapFrame) {
        thrift.__set_mapFrame(mapFrame.value().toThrift());
    }
    return thrift;
}
}
}