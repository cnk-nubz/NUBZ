#ifndef SERVER_IO_INPUT__SET_EXHIBIT_FRAME_REQUEST__H
#define SERVER_IO_INPUT__SET_EXHIBIT_FRAME_REQUEST__H

#include <cstdint>

#include <communication/structs_types.h>

#include <server/io/Frame.h>

namespace server {
namespace io {
namespace input {

struct SetExhibitFrameRequest {
    SetExhibitFrameRequest(const communication::SetExhibitFrameRequest &thrift);

    std::int32_t exhibitId;
    Frame frame;
};
}
}
}

#endif