#ifndef SERVER_COMMAND__EXPERIMENT_COMMANDS__H
#define SERVER_COMMAND__EXPERIMENT_COMMANDS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/Database.h>

#include <repository/Experiments.h>

#include <server/io/input/CreateExperimentRequest.h>
#include <server/io/input/QuestionsIdsList.h>
#include <server/io/output/CurrentExperimentResponse.h>
#include <server/io/output/SingleExperimentInfo.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class ExperimentCommands : public Command {
public:
    ExperimentCommands(db::Database &db);

    void create(const CreateExperimentRequest &input);
    void update(std::int32_t ID, const CreateExperimentRequest &input);

    void finish();
    void start(std::int32_t ID);

    // get for www
    SingleExperimentInfo getActive();
    std::vector<ExperimentInfo> getAllReady();
    std::vector<ExperimentInfo> getAllFinished();
    Experiment get(std::int32_t ID);

    // get for android
    CurrentExperimentResponse getCurrent();

private:
    repository::Experiments::LazyExperiment toRepo(const CreateExperimentRequest &request) const;
    repository::Experiments::LazyExperiment::Survey toRepoSurvey(
        const QuestionsIdsList &qList) const;

    db::Database &db;
};
}
}

#endif