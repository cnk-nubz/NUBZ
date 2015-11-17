#ifndef COMMAND__PING_COMMAND__H
#define COMMAND__PING_COMMAND__H

#include "AbstractCommand.h"
#include "io/input/HelloMsg.h"

namespace command {
    class PingCommand : public AbstractCommand<io::input::HelloMsg, std::int32_t> {
    public:
        virtual std::int32_t perform(const io::input::HelloMsg &input) override;
    };
}

#endif