#ifndef REPOSITORY__INVALID_DATA__H
#define REPOSITORY__INVALID_DATA__H

namespace repository {
struct InvalidData : std::invalid_argument {
    using invalid_argument::invalid_argument;
};
}

#endif