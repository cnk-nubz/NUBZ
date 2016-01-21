#include "Experiment.h"

namespace server {
namespace io {
namespace output {

communication::Experiment Experiment::toThrift() const {
    communication::Experiment res;
    res.experimentId = experimentID;
    res.name = name;
    res.exhibitActions = toThriftActions(exhibitActions);
    res.breakActions = toThriftActions(breakActions);
    return res;
}

std::vector<communication::Action> Experiment::toThriftActions(
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