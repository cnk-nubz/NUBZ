#ifndef REPOSITORY_ERROR__IN_USE__H
#define REPOSITORY_ERROR__IN_USE__H

namespace repository {
struct InUse : std::invalid_argument {
    using invalid_argument::invalid_argument;
};
}

#endif