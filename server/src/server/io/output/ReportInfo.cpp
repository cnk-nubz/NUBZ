#include "ReportInfo.h"

namespace server {
namespace io {
namespace output {

ReportInfo::ReportInfo(const repository::Reports::ReportInfo &repo)
    : reportID(repo.ID), receiveDate(repo.receiveDate) {
}

communication::ReportInfo ReportInfo::toThrift() const {
    auto res = communication::ReportInfo{};
    res.reportId = reportID;
    res.receiveDate = receiveDate.toThrift();
    return res;
}
}
}
}