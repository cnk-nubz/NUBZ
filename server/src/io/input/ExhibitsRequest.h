#ifndef IO__EXHIBITS_REQUEST__H
#define IO__EXHIBITS_REQUEST__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include "communication/structs_types.h"

namespace io {
    namespace input {
        class ExhibitsRequest {
        public:
            ExhibitsRequest() = default;
            ~ExhibitsRequest() = default;

            ExhibitsRequest(const ExhibitsRequest &) = default;
            ExhibitsRequest(ExhibitsRequest &&) = default;
            ExhibitsRequest &operator=(const ExhibitsRequest &) = default;
            ExhibitsRequest &operator=(ExhibitsRequest &&) = default;

            ExhibitsRequest(const communication::ExhibitsRequest &thrift);

            boost::optional<std::int32_t> acquiredVersion;
        };
    }
}

#endif