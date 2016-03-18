#ifndef SERVER_IO_OUTPUT__SINGLE_EXPERIMENT_INFO__H
#define SERVER_IO_OUTPUT__SINGLE_EXPERIMENT_INFO__H

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include "ExperimentInfo.h"

namespace server {
namespace io {
namespace output {

struct SingleExperimentInfo {
    using thrift_t = communication::SingleExperimentInfo;
    using repo_t = repository::Experiments::LazyExperiment;

    SingleExperimentInfo() = default;
    SingleExperimentInfo(const boost::optional<repo_t> &repo);

    thrift_t toThrift() const;

    const boost::optional<ExperimentInfo> info;
};
}
}
}

#endif