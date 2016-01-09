#include "GetExperimentDataCommand.h"
#include "db/command/GetActions.h"

namespace command {
    GetExperimentDataCommand::GetExperimentDataCommand(db::Database &db) : db(db) {
    }
    
    io::output::ExperimentData GetExperimentDataCommand::operator()() {
        io::output::ExperimentData result;
        
        db.execute([&](db::DatabaseSession &session) {
            result.exhibitActions = getActions(session, false);
            result.breakActions = getActions(session, true);
        });
        
        return result;
    }
    
    std::vector<io::Action> GetExperimentDataCommand::getActions(db::DatabaseSession &session, bool duringBreak) const {
        db::cmd::GetActions cmd;
        cmd.duringBreakCondition = duringBreak;
        cmd.inExperimentCondition = true;
        cmd(session);
        
        std::vector<io::Action> res;
        for (const auto &rawAction : cmd.getResult()) {
            res.emplace_back(rawAction);
        }
        return res;
    }
}