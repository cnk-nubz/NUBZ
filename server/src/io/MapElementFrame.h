#ifndef IO__MAP_ELEMENT_FRAME__H
#define IO__MAP_ELEMENT_FRAME__H

#include <string>

#include <boost/optional.hpp>

#include "communication/structs_types.h"
#include "db/struct/MapElementFrame.h"

namespace io {
    struct MapElementFrame {
        MapElementFrame() = default;
        ~MapElementFrame() = default;
        
        MapElementFrame(const MapElementFrame &) = default;
        MapElementFrame(MapElementFrame &&) = default;
        MapElementFrame &operator=(const MapElementFrame &) = default;
        MapElementFrame &operator=(MapElementFrame &&) = default;
        
        MapElementFrame(const communication::MapElementFrame &thrift);
        communication::MapElementFrame toThrift() const;
        
        MapElementFrame(const db::MapElementFrame &dbMapElementFrame);
        db::MapElementFrame toDb() const;
        
        std::int32_t x;
        std::int32_t y;
        std::int32_t width;
        std::int32_t height;
        std::int32_t mapLevel;
    };
}

#endif