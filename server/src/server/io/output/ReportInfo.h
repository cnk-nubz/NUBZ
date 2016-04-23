#ifndef SERVER_IO_OUTPUT__REPORT_INFO__H
#define SERVER_IO_OUTPUT__REPORT_INFO__H

#include <communication/structs_types.h>

#include <repository/Reports.h>

#include "Date.h"

namespace server {
namespace io {
namespace output {

struct ReportInfo {
    using thrift_t = communication::ReportInfo;
    using repo_t = repository::Reports::ReportInfo;

    ReportInfo() = default;
    thrift_t toThrift() const;

    ReportInfo(const repo_t &repo);

    std::int32_t reportID;
    Date receiveDate;
};
}
}
}

#endif