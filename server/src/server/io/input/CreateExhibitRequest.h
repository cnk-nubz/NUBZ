#ifndef SERVER_IO_INPUT__CREATE_EXHIBIT_REQUEST__H
#define SERVER_IO_INPUT__CREATE_EXHIBIT_REQUEST__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <server/io/MapFrame.h>

namespace server {
namespace io {
namespace input {

struct CreateExhibitRequest {
    CreateExhibitRequest(const communication::CreateExhibitRequest &thrift);

    std::string name;
    std::int32_t rgbHex;
    boost::optional<std::int32_t> floor;
    boost::optional<MapFrame> visibleFrame;
};
}
}
}

#endif