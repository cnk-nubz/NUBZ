#ifndef SERVER_IO__RAW_REPORT__H
#define SERVER_IO__RAW_REPORT__H

#include <cstdint>
#include <iostream>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

class RawReport {
public:
    class Event {
    public:
        Event(const communication::RawReportEvent &thrift);

        std::vector<std::int32_t> actions;
        std::int32_t durationInSecs;

        // action without exhibitId is a break action
        boost::optional<std::int32_t> exhibitId;
    };

    RawReport(const communication::RawReport &thrift);

    std::int32_t experimentId;
    std::int32_t reportId;
    std::vector<Event> history;
};
}
}
}

#endif