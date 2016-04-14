#ifndef UTILS_EXCEL_COLUMN__EVENT_BASED__H
#define UTILS_EXCEL_COLUMN__EVENT_BASED__H

#include <repository/Reports.h>

#include "Column.h"

namespace utils {
namespace excel {

class EventBased : public Column {
public:
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const = 0;

protected:
    using Column::Column;
};
}
}

#endif