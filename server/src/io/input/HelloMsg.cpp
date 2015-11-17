#include "HelloMsg.h"

namespace io {
    namespace input {
        HelloMsg::HelloMsg(const communication::HelloMsg &thrift)
            : num(thrift.num), msg(thrift.msg) {
        }
    }
}