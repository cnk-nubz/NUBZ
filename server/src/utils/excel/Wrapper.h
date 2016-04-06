#ifndef UTILS_EXCEL__WRAPPER__H
#define UTILS_EXCEL__WRAPPER__H

#include <iostream>
#include <memory>

namespace utils {
namespace excel {

class Wrapper {
public:
    virtual ~Wrapper() = default;

    virtual void print(std::ostream &ostream) const = 0;
};

using wrapper_ptr = std::unique_ptr<Wrapper>;
}
}

#endif