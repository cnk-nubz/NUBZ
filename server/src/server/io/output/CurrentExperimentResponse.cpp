#include "CurrentExperimentResponse.h"

namespace server {
namespace io {
namespace output {

communication::CurrentExperimentResponse CurrentExperimentResponse::toThrift() const {
    communication::CurrentExperimentResponse res;
    if (experiment) {
        res.__set_experiment(experiment.value().toThrift());
    }
    return res;
}
}
}
}