#ifndef IO__EXHIBITS_RESPONSE__H
#define IO__EXHIBITS_RESPONSE__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include "communication/structs_types.h"
#include "io/Exhibit.h"

namespace io {
    namespace output {
        struct ExhibitsResponse {
            communication::ExhibitsResponse toThrift() const;

            std::int32_t version;
            std::map<std::int32_t, Exhibit> exhibits;
        };
    }
}

#endif