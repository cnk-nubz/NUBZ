#ifndef SERVER_IO__SET_EXHIBIT_FRAME_REQUEST__H
#define SERVER_IO__SET_EXHIBIT_FRAME_REQUEST__H

#include <cstdint>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class SetExhibitFrameRequest {
public:
    SetExhibitFrameRequest(const communication::SetExhibitFrameRequest &thrift);

    std::int32_t exhibitId;
    std::int32_t newX;
    std::int32_t newY;
    std::int32_t newWidth;
    std::int32_t newHeight;
};
}
}
}

#endif