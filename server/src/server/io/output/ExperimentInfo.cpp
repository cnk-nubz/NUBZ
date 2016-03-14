#include "ExperimentInfo.h"

namespace server {
namespace io {
namespace output {

ExperimentInfo::ExperimentInfo(const repository::Experiments::LazyExperiment &repo)
    : ID(repo.ID), name(repo.name), startDate(repo.startDate), finishDate(repo.finishDate) {
}

communication::ExperimentInfo ExperimentInfo::toThrift() const {
    auto res = communication::ExperimentInfo{};
    res.id = ID;
    res.name = name;
    if (startDate) {
        res.__set_startDate(startDate.value().toThrift());
    }
    if (finishDate) {
        res.__set_finishDate(finishDate.value().toThrift());
    }
    return res;
}
}
}
}