#ifndef IO__SET_EXHIBIT_FRAME_REQUEST__H
#define IO__SET_EXHIBIT_FRAME_REQUEST__H

#include <cstdint>
#include <iostream>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class SetExhibitFrameRequest {
        public:
            SetExhibitFrameRequest() = default;
            ~SetExhibitFrameRequest() = default;

            SetExhibitFrameRequest(const communication::SetExhibitFrameRequest &thrift);

            SetExhibitFrameRequest(const SetExhibitFrameRequest &) = default;
            SetExhibitFrameRequest(SetExhibitFrameRequest &&) = default;
            SetExhibitFrameRequest &operator=(const SetExhibitFrameRequest &) = default;
            SetExhibitFrameRequest &operator=(SetExhibitFrameRequest &&) = default;

            std::int32_t exhibitId;
            std::int32_t newX;
            std::int32_t newY;
            std::int32_t newWidth;
            std::int32_t newHeight;
        };
    }
}

#endif