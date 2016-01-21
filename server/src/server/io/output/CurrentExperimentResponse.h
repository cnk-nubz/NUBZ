#ifndef IO__CURRENT_EXPERIMENT_RESPONSE__H
#define IO__CURRENT_EXPERIMENT_RESPONSE__H

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include "Experiment.h"

namespace server {
namespace io {
namespace output {

class CurrentExperimentResponse {
public:
    communication::CurrentExperimentResponse toThrift() const;

    boost::optional<Experiment> experiment;
};
}
}
}

#endif