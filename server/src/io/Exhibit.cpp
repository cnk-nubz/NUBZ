#include "Exhibit.h"

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

    Exhibit::Exhibit(const db::Exhibit &dbExhibit) : name(dbExhibit.name) {
        if (dbExhibit.frame) {
            frame = MapElementFrame(dbExhibit.frame.value());
        }
    }

    db::Exhibit Exhibit::toDb() const {
        db::Exhibit exhibit;
        exhibit.ID = -1;
        exhibit.name = name;
        if (frame) {
            exhibit.frame = frame.value().toDb();
        }
        return exhibit;
    }
}