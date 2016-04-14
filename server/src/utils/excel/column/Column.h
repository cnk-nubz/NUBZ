#ifndef UTILS_EXCEL_COLUMN__COLUMN__H
#define UTILS_EXCEL_COLUMN__COLUMN__H

#include <memory>

#include <utils/excel/Wrapper.h>

namespace utils {
namespace excel {

class Column {
public:
    virtual ~Column() = default;

    virtual wrapper_ptr getHeader() const;

protected:
    Column();

    // pointer cannot be freed before getHeader call
    Column(const char *title);

private:
    const char *title;
};
}
}

#endif