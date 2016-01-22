#ifndef SERVER_UTILS__INPUT_CHECKER__H
#define SERVER_UTILS__INPUT_CHECKER__H

#include <unordered_set>
#include <algorithm>
#include <functional>

#include <utils/fp_algorithm.h>

#include <db/DatabaseSession.h>
#include <db/command/GetExhibits.h>
#include <db/command/GetActions.h>
#include <db/struct/Experiment.h>

#include <server/command/commons.h>

namespace server {
namespace utils {

class InputChecker {
public:
    InputChecker(db::DatabaseSession &session);
    SRV_CMD_CP_MV(InputChecker);

    bool checkReportId(std::int32_t reportId);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkExhibitsIds(const Container &exhibitsIds);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkActionsIds(const Container &actionsIds);

    bool checkFrame(std::int32_t floor, std::int32_t x, std::int32_t y, std::int32_t width,
                    std::int32_t height);

    bool checkText(const std::string &text);

private:
    db::DatabaseSession &session;
};

template <class Container, class>
bool InputChecker::checkExhibitsIds(const Container &exhibitsIds) {
    using std::placeholders::_1;
    std::unordered_set<std::int32_t> correctIds;
    for (const auto &exhibit : db::cmd::GetExhibits{}(session)) {
        correctIds.insert(exhibit.ID);
    }
    return ::utils::all_of(exhibitsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
}

template <class Container, class>
bool InputChecker::checkActionsIds(const Container &actionsIds) {
    using std::placeholders::_1;
    std::unordered_set<std::int32_t> correctIds;
    for (const auto &action : db::cmd::GetActions{}(session)) {
        correctIds.insert(action.ID);
    }
    return ::utils::all_of(actionsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
}
}
}

#endif