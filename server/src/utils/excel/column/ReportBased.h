#ifndef UTILS_EXCEL_COLUMN__REPORT_BASED__H
#define UTILS_EXCEL_COLUMN__REPORT_BASED__H

#include <repository/Reports.h>

#include "Column.h"

namespace utils {
namespace excel {

class ReportBased : public Column {
public:
    virtual wrapper_ptr getEntry(const repository::Report &report) const = 0;

protected:
    using Column::Column;
};
}
}

#endif