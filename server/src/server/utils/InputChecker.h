#ifndef SERVER_UTILS__INPUT_CHECKER__H
#define SERVER_UTILS__INPUT_CHECKER__H

#include <unordered_set>
#include <algorithm>

#include <db/DatabaseSession.h>
#include <db/command/GetExhibits.h>
#include <db/command/GetActions.h>

#include <server/command/commons.h>

namespace server {
namespace utils {

class InputChecker {
public:
    InputChecker(db::DatabaseSession &session);
    ~InputChecker() = default;

    SRV_CMD_CP_MV(InputChecker);

    bool checkReportId(std::int32_t reportId);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkExhibitsIds(const Container &exhibitsIds);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkExhibitsActionsIds(const Container &actionsIds);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkBreakActionsIds(const Container &actionsIds);

    bool checkFrame(std::int32_t floor, std::int32_t x, std::int32_t y, std::int32_t width,
                    std::int32_t height);

    bool checkText(const std::string &text);

private:
    db::DatabaseSession &session;
};

template <class Container, class>
bool InputChecker::checkExhibitsIds(const Container &exhibitsIds) {
    static_assert(std::is_same<typename Container::value_type, std::int32_t>::value,
                  "exhibit id type should be std::int32_t");

    std::unordered_set<std::int32_t> correctIds;
    for (const auto &exhibit : db::cmd::GetExhibits{}(session)) {
        correctIds.insert(exhibit.ID);
    }

    return std::all_of(exhibitsIds.begin(), exhibitsIds.end(), [&](auto exhibitId) {
        return correctIds.count(exhibitId) > 0;
    });
}

template <class Container, class>
bool InputChecker::checkExhibitsActionsIds(const Container &actionsIds) {
    static_assert(std::is_same<typename Container::value_type, std::int32_t>::value,
                  "action id type should be std::int32_t");

    std::unordered_set<std::int32_t> correctIds;
    for (const auto &action : db::cmd::GetActions{}(session)) {
        correctIds.insert(action.ID);
    }

    return std::all_of(actionsIds.begin(), actionsIds.end(), [&](auto actionId) {
        return correctIds.count(actionId) > 0;
    });
}

template <class Container, class>
bool InputChecker::checkBreakActionsIds(const Container &) {
    return true;
}
}
}

#endif