#include <utils/fp_algorithm.h>

#include <repository/Actions.h>
#include <repository/Experiments.h>

#include "GetCurrentExperimentCommand.h"

namespace server {
namespace command {

GetCurrentExperimentCommand::GetCurrentExperimentCommand(db::Database &db) : db(db) {
}

io::output::CurrentExperimentResponse GetCurrentExperimentCommand::operator()() {
    boost::optional<repository::Experiment> repoExperiment =
        db.execute([&](db::DatabaseSession &session) {
            auto repo = repository::Experiments{session};
            return repo.getActive();
        });

    auto result = io::output::CurrentExperimentResponse{};
    if (repoExperiment) {
        result.experiment = io::output::Experiment{repoExperiment.value()};
    }
    return result;
}
}
}