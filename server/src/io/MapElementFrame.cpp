#include "MapElementFrame.h"

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

    MapElementFrame::MapElementFrame(const db::MapElementFrame &dbMapElementFrame)
        : x(dbMapElementFrame.x),
          y(dbMapElementFrame.y),
          width(dbMapElementFrame.width),
          height(dbMapElementFrame.height),
          mapLevel(dbMapElementFrame.mapLevel) {
    }

    db::MapElementFrame MapElementFrame::toDb() const {
        db::MapElementFrame frame;
        frame.x = x;
        frame.y = y;
        frame.width = width;
        frame.height = height;
        frame.mapLevel = mapLevel;
        return frame;
    }
}