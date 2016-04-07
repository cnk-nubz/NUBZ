#include <cassert>

#include <utils/excel/WrapperFactory.h>

#include "Column.h"

namespace utils {
namespace excel {

Column::Column() : title(nullptr) {
}

Column::Column(const char *title) : title(title) {
}

wrapper_ptr Column::getHeader() const {
    assert(title);
    return HeaderWrapperFactory::stringWrapper(title);
}
}
}