#include <utils/fp_algorithm.h>

#include <db/command/GetActions.h>
#include <db/command/GetCurrentExperiment.h>

#include <server/utils/io_translation.h>

#include "GetCurrentExperimentCommand.h"

namespace server {
namespace command {

GetCurrentExperimentCommand::GetCurrentExperimentCommand(db::Database &db) : db(db) {
}

io::output::CurrentExperimentResponse GetCurrentExperimentCommand::operator()() {
    io::output::CurrentExperimentResponse response;
    db.execute([&](db::DatabaseSession &session) {
        boost::optional<db::Experiment> experimentOpt = db::cmd::GetCurrentExperiment{}(session);
        if (!experimentOpt) {
            return;
        }

        auto experiment = experimentOpt.value();
        currentExperiment.ID = experiment.ID;
        currentExperiment.name = experiment.name;
        fillExperimentData(experiment, session);

        response.experiment = currentExperiment;
    });

    return response;
}

void GetCurrentExperimentCommand::fillExperimentData(const db::Experiment &experiment,
                                                     db::DatabaseSession &session) {
    auto getActions = [&](std::int32_t actionId) {
        return utils::toIO(db::cmd::GetActions{actionId}(session).front());
    };

    ::utils::transform(experiment.actions, currentExperiment.exhibitActions, getActions);
    ::utils::transform(experiment.breakActions, currentExperiment.breakActions, getActions);
}
}
}