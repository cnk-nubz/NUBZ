#ifndef SERVER_COMMAND__COMMAND__H
#define SERVER_COMMAND__COMMAND__H

namespace server {
namespace command {

class Command {
public:
    Command() = default;
    Command(const Command &) = delete;
    Command(Command &&) = default;
    Command &operator=(const Command &) = delete;
    Command &operator=(Command &&) = default;
};
}
}

#endif