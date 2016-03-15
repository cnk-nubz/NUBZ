#ifndef SERVER_IO_OUTPUT__EXPERIMENT_INFO__H
#define SERVER_IO_OUTPUT__EXPERIMENT_INFO__H

#include <cstdint>
#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <repository/Experiments.h>

#include "Date.h"

namespace server {
namespace io {
namespace output {

struct ExperimentInfo {
    using thrift_t = communication::ExperimentInfo;
    using repo_t = repository::Experiments::LazyExperiment;

    ExperimentInfo(const repo_t &repo);

    thrift_t toThrift() const;

    const std::int32_t ID;
    const std::string name;
    const boost::optional<Date> startDate;
    const boost::optional<Date> finishDate;
};
}
}
}

#endif