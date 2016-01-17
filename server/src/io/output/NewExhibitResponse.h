#ifndef IO__NEW_EXHIBIT_RESPONSE__H
#define IO__NEW_EXHIBIT_RESPONSE__H

#include <cstdint>

#include "communication/structs_types.h"
#include "io/Exhibit.h"

namespace io {
    namespace output {
        class NewExhibitResponse {
        public:
            communication::NewExhibitResponse toThrift() const;

            std::int32_t exhibitId;
            io::Exhibit exhibit;
        };
    }
}

#endif