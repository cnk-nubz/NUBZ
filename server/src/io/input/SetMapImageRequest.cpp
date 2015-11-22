#include "SetMapImageRequest.h"

namespace io {
    namespace input {
        SetMapImageRequest::SetMapImageRequest(const communication::SetMapImageRequest &thrift) {
            level = thrift.level;
            filename = thrift.filename;
        }
    }
}