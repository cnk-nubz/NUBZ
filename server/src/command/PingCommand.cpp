#include "PingCommand.h"

namespace command {

std::int32_t PingCommand::perform(const io::input::HelloMsg &input) {
    return input.num;
}
}