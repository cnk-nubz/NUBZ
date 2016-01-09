#include "GetActions.h"
#include "db/db_info.h"
#include "db/factory/ActionFactory.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        void GetActions::operator()(db::DatabaseSession &session) {
            result = session.getResults<db::factory::ActionFactory>(createQuery());
        }
        
        const std::vector<Action> &GetActions::getResult() const {
            return result;
        }
        
        std::string GetActions::createQuery() const {
            using namespace db::info::actions;
            using namespace db::sql;
            
            auto select = Sql::select(db::factory::ActionFactory::fieldsOrder()).from(tableName);
            if (inExperimentCondition) {
                select.where(C(colInCurrentExperiment) == inExperimentCondition.value());
            }
            if (duringBreakCondition) {
                select.where(C(colDuringBreak) == duringBreakCondition.value());
            }
            
            return select;
        }
    }
}