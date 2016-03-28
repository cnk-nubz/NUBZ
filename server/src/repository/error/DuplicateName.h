#ifndef REPOSITORY_ERROR__DUPLICATE_NAME__H
#define REPOSITORY_ERROR__DUPLICATE_NAME__H

namespace repository {
struct DuplicateName : std::invalid_argument {
    using invalid_argument::invalid_argument;

    inline DuplicateName() : std::invalid_argument("") {
    }
};
}

#endif