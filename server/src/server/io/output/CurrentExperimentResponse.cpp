#include "CurrentExperimentResponse.h"

namespace server {
namespace io {
namespace output {

communication::CurrentExperimentResponse CurrentExperimentResponse::toThrift() const {
    auto res = communication::CurrentExperimentResponse{};
    if (experiment) {
        res.__set_experiment(experiment.value().toThrift());
    }
    return res;
}
}
}
}