#include "SingleExperimentInfo.h"

namespace server {
namespace io {
namespace output {

SingleExperimentInfo::SingleExperimentInfo(
    const boost::optional<repository::Experiments::LazyExperiment> &repo)
    : info(repo) {
}

communication::SingleExperimentInfo SingleExperimentInfo::toThrift() const {
    auto res = communication::SingleExperimentInfo{};
    if (info) {
        res.__set_info(info.value().toThrift());
    }
    return res;
}
}
}
}