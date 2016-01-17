#ifndef IO__NEW_EXHIBIT_REQUEST__H
#define IO__NEW_EXHIBIT_REQUEST__H

#include <cstdint>

#include <boost/optional.hpp>

#include "communication/structs_types.h"
#include "io/MapElementFrame.h"

namespace io {
    namespace input {
        class NewExhibitRequest {
        public:
            NewExhibitRequest() = default;
            ~NewExhibitRequest() = default;

            NewExhibitRequest(const communication::NewExhibitRequest &thrift);

            NewExhibitRequest(const NewExhibitRequest &) = default;
            NewExhibitRequest(NewExhibitRequest &&) = default;
            NewExhibitRequest &operator=(const NewExhibitRequest &) = default;
            NewExhibitRequest &operator=(NewExhibitRequest &&) = default;

            std::string name;
            boost::optional<std::int32_t> floor;
            boost::optional<io::MapElementFrame> visibleMapFrame;
        };
    }
}

#endif