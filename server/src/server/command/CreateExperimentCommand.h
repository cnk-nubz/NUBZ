#ifndef SERVER_COMMAND__CREATE_EXPERIMENT_COMMAND__H
#define SERVER_COMMAND__CREATE_EXPERIMENT_COMMAND__H

#include <db/Database.h>

#include <repository/Experiments.h>

#include <server/io/input/CreateExperimentRequest.h>

#include "Command.h"

namespace server {
namespace command {

class CreateExperimentCommand : public Command {
public:
    CreateExperimentCommand(db::Database &db);

    void operator()(const io::input::CreateExperimentRequest &input);

private:
    repository::Experiments::LazyExperiment::Survey toRepoSurvey(
        const io::input::QuestionsIdsList &qList);

    db::Database &db;
};
}
}

#endif