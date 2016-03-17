#ifndef SERVER_IO_OUTPUT__ACTION__H
#define SERVER_IO_OUTPUT__ACTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include <repository/Actions.h>

namespace server {
namespace io {
namespace output {

struct Action {
    using thrift_t = communication::Action;
    using repo_t = repository::Action;

    Action(const repo_t &repo);

    thrift_t toThrift() const;

    std::int32_t ID;
    std::string text;
};
}
}
}

#endif