#ifndef COMMAND__INPUT_CHECKER__H
#define COMMAND__INPUT_CHECKER__H

#include "commands_common.h"
#include "db/DatabaseSession.h"
#include "db/command/GetExhibits.h"
#include "db/command/GetActions.h"

namespace command {
    class InputChecker {
    public:
        InputChecker(db::DatabaseSession &session);
        ~InputChecker() = default;

        DEFAULT_CP_MV(InputChecker);

        bool checkReportId(std::int32_t reportId);

        template <class Container>
        bool checkExhibitsIds(const Container &exhibitsIds);

        template <class Container>
        bool checkExhibitsActionsIds(const Container &actionsIds);

        template <class Container>
        bool checkBreakActionsIds(const Container &actionsIds);

    private:
        db::DatabaseSession &session;
    };

    template <class Container>
    bool InputChecker::checkExhibitsIds(const Container &exhibitsIds) {
        static_assert(std::is_same<typename Container::value_type, std::int32_t>::value,
                      "exhibit id type should be std::int32_t");

        db::cmd::GetExhibits cmd;
        cmd(session);
        
        std::unordered_set<std::int32_t> correctIds;
        for (const auto &exhibit : cmd.getResult()) {
            correctIds.insert(exhibit.ID);
        }

        for (const auto eId : exhibitsIds) {
            if (!correctIds.count(eId)) {
                return false;
            }
        }
        return true;
    }

    template <class Container>
    bool InputChecker::checkExhibitsActionsIds(const Container &actionsIds) {
        static_assert(std::is_same<typename Container::value_type, std::int32_t>::value,
                      "action id type should be std::int32_t");

        db::cmd::GetActions cmd;
        cmd.duringBreakCondition = false;
        cmd(session);
        
        std::unordered_set<std::int32_t> correctIds;
        for (const auto &action : cmd.getResult()) {
            correctIds.insert(action.ID);
        }
        
        for (const auto actionId : actionsIds) {
            if (!correctIds.count(actionId)) {
                return false;
            }
        }
        
        return true;
    }

    template <class Container>
    bool InputChecker::checkBreakActionsIds(const Container &actionsIds) {
        static_assert(std::is_same<typename Container::value_type, std::int32_t>::value,
                      "action id type should be std::int32_t");

        db::cmd::GetActions cmd;
        cmd.duringBreakCondition = true;
        cmd(session);
        
        std::unordered_set<std::int32_t> correctIds;
        for (const auto &action : cmd.getResult()) {
            correctIds.insert(action.ID);
        }
        
        for (const auto actionId : actionsIds) {
            if (!correctIds.count(actionId)) {
                return false;
            }
        }
        
        return true;
    }
}

#endif