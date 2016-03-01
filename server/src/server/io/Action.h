#ifndef SERVER_IO__ACTION__H
#define SERVER_IO__ACTION__H

#include <string>
#include <cstdint>

#include <communication/structs_types.h>

#include <repository/Actions.h>

namespace server {
namespace io {

struct Action {
    using thrift_t = communication::Action;
    using repo_t = repository::Action;

    Action() = default;

    Action(const thrift_t &thrift);
    thrift_t toThrift() const;

    Action(const repo_t &repo);

    std::int32_t ID;
    std::string text;
};
}
}

#endif