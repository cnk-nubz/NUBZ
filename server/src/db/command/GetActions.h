#ifndef DB_CMD__GET_ACTIONS__H
#define DB_CMD__GET_ACTIONS__H

#include <boost/optional.hpp>

#include "db/DatabaseSession.h"
#include "db/struct/Action.h"

namespace db {
    namespace cmd {
        class GetActions {
        public:
            GetActions() = default;
            ~GetActions() = default;
            
            void operator()(DatabaseSession &session);
            const std::vector<Action> &getResult() const;
            
            boost::optional<bool> inExperimentCondition;
            boost::optional<bool> duringBreakCondition;
            
        private:
            std::vector<db::Action> result;
            
            std::string createQuery() const;
        };
    }
}

#endif