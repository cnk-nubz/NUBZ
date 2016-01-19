#ifndef SERVER_IO__NEW_EXHIBIT_REQUEST__H
#define SERVER_IO__NEW_EXHIBIT_REQUEST__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>
#include <server/io/MapElementFrame.h>

namespace server {
namespace io {
namespace input {

class NewExhibitRequest {
public:
    NewExhibitRequest(const communication::NewExhibitRequest &thrift);

    std::string name;
    boost::optional<std::int32_t> floor;
    boost::optional<io::MapElementFrame> visibleMapFrame;
};
}
}
}

#endif