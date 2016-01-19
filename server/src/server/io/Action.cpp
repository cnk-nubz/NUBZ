#include "Action.h"

namespace server {
namespace io {

Action::Action(const communication::Action &thrift) : ID(thrift.actionId), text(thrift.text) {
}

communication::Action Action::toThrift() const {
    communication::Action thrift;
    thrift.actionId = ID;
    thrift.text = text;
    return thrift;
}
}
}