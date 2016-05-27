#ifndef SERVER_IO_INPUT__UPDATE_EXHIBIT_REQUEST__H
#define SERVER_IO_INPUT__UPDATE_EXHIBIT_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <server/io/MapFrame.h>

namespace server {
namespace io {
namespace input {

struct UpdateExhibitRequest {
    UpdateExhibitRequest(const communication::UpdateExhibitRequest &thrift);

    std::int32_t exhibitId;
    std::int32_t rgbHex;
    boost::optional<MapFrame> visibleFrame;
    boost::optional<std::int32_t> floor;
};
}
}
}

#endif