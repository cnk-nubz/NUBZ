#include "RawReport.h"

namespace server {
namespace io {
namespace input {

RawReport::RawReport(const communication::RawReport &thrift) : reportId(thrift.reportId) {
    for (const auto &event : thrift.history) {
        history.emplace_back(event);
    }
}

RawReport::Event::Event(const communication::RawReportEvent &thrift)
    : actions(thrift.actions), durationInSecs(thrift.durationInSecs) {
    if (thrift.__isset.exhibitId) {
        exhibitId = thrift.exhibitId;
    }
}
}
}
}