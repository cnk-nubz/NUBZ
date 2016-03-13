#include "Action.h"

namespace server {
namespace io {
namespace output {

Action::Action(const communication::Action &thrift) : ID(thrift.actionId), text(thrift.text) {
}

communication::Action Action::toThrift() const {
    auto res = communication::Action{};
    res.actionId = ID;
    res.text = text;
    return res;
}

Action::Action(const repository::Action &repo) : ID(repo.ID), text(repo.text) {
}
}
}
}