#include "HelloMsg.h"

namespace server {
namespace io {
namespace input {

HelloMsg::HelloMsg(const communication::HelloMsg &thrift) : num(thrift.num), msg(thrift.msg) {
}
}
}
}