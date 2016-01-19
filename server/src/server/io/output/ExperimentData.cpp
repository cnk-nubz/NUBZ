#include "ExperimentData.h"

namespace server {
namespace io {
namespace output {

communication::ExperimentData ExperimentData::toThrift() const {
    communication::ExperimentData res;
    res.exhibitActions = toThriftActions(exhibitActions);
    res.breakActions = toThriftActions(breakActions);
    return res;
}

std::vector<communication::Action> ExperimentData::toThriftActions(
    const std::vector<io::Action> &ioActions) const {
    std::vector<communication::Action> res;
    for (const auto &ioAction : ioActions) {
        res.push_back(ioAction.toThrift());
    }
    return res;
}
}
}
}