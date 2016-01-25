#include <boost/none.hpp>

#include <db/command/GetExperiments.h>
#include <db/command/GetCurrentExperiment.h>

#include "ReportChecker.h"

namespace server {
namespace utils {

ReportChecker::ReportChecker(db::DatabaseSession &session) : session(session) {
}

bool ReportChecker::loadCurrentExperiment() {
    experiment = db::cmd::GetCurrentExperiment{}(session);
    return (bool)experiment;
}

bool ReportChecker::loadExperiment(std::int32_t experimentId) {
    db::cmd::GetExperiments getExperiments(experimentId);
    getExperiments(session);
    if (!getExperiments.getResult().empty()) {
        experiment = getExperiments.getResult().front();
    } else {
        experiment = boost::none;
    }
    return (bool)experiment;
}
}
}