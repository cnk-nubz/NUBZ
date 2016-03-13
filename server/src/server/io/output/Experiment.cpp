#include <utils/fp_algorithm.h>

#include <server/io/utils.h>

#include "Experiment.h"

namespace server {
namespace io {
namespace output {

Experiment::Experiment(const repository::Experiment &repo)
    : ID(repo.ID),
      name(repo.name),
      exhibitActions(std::vector<Action>(repo.actions.begin(), repo.actions.end())),
      breakActions(std::vector<Action>(repo.breakActions.begin(), repo.breakActions.end())),
      surveyBefore(repo.surveyBefore),
      surveyAfter(repo.surveyAfter) {
}

communication::Experiment Experiment::toThrift() const {
    auto res = communication::Experiment{};
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