#include <utils/fp_algorithm.h>

#include <server/io/utils.h>

#include "Experiment.h"

namespace server {
namespace io {
namespace output {

communication::Experiment Experiment::toThrift() const {
    communication::Experiment res;
    res.experimentId = ID;
    res.name = name;
    res.exhibitActions = ioToThrift(exhibitActions);
    res.breakActions = ioToThrift(breakActions);
    res.surveyBefore = surveyBefore.toThrift();
    res.surveyAfter = surveyAfter.toThrift();
    return res;
}
}
}
}