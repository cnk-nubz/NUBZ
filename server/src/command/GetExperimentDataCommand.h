#ifndef COMMAND__GET_EXPERIMENT_DATA_COMMAND__H
#define COMMAND__GET_EXPERIMENT_DATA_COMMAND__H

#include "commands_common.h"
#include "io/output/ExperimentData.h"
#include "db/Database.h"

namespace command {
    class GetExperimentDataCommand {
    public:
        GetExperimentDataCommand(db::Database &db);
        ~GetExperimentDataCommand() = default;
        
        DEFAULT_CP_MV(GetExperimentDataCommand);
        
        io::output::ExperimentData operator()();
        
    private:
        db::Database &db;
        
        std::vector<io::Action> getActions(db::DatabaseSession &session, bool duringBreak) const;
    };
}

#endif