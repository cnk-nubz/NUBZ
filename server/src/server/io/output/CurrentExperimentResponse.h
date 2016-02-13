#ifndef SERVER_IO_OUTPUT__CURRENT_EXPERIMENT_RESPONSE__H
#define SERVER_IO_OUTPUT__CURRENT_EXPERIMENT_RESPONSE__H

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include "Experiment.h"

namespace server {
namespace io {
namespace output {

struct CurrentExperimentResponse {
    communication::CurrentExperimentResponse toThrift() const;

    boost::optional<Experiment> experiment;
};
}
}
}

#endif