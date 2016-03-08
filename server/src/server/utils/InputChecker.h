#ifndef SERVER_UTILS__INPUT_CHECKER__H
#define SERVER_UTILS__INPUT_CHECKER__H

#include <algorithm>
#include <functional>
#include <unordered_set>

#include <utils/fp_algorithm.h>

#include <repository/Actions.h>
#include <repository/Exhibits.h>

#include <db/DatabaseSession.h>

#include <server/command/commons.h>

namespace server {
namespace utils {

class InputChecker {
public:
    InputChecker(db::DatabaseSession &session);
    SRV_CMD_CP_MV(InputChecker);

    static bool checkText(const std::string &text);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkExhibitsIds(const Container &exhibitsIds);

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkActionsIds(const Container &actionsIds);

    bool checkFrame(std::int32_t floor, std::int32_t x, std::int32_t y, std::int32_t width,
                    std::int32_t height);

private:
    db::DatabaseSession &session;
};

template <class Container, class>
bool InputChecker::checkExhibitsIds(const Container &exhibitsIds) {
    auto correctIds = std::unordered_set<std::int32_t>{};
    for (const auto &exhibit : repository::Exhibits{session}.getAll()) {
        correctIds.insert(exhibit.ID);
    }
    return ::utils::all_of(exhibitsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
}

template <class Container, class>
bool InputChecker::checkActionsIds(const Container &actionsIds) {
    auto correctIds = std::unordered_set<std::int32_t>{};
    for (const auto &action : repository::Actions{session}.getAll()) {
        correctIds.insert(action.ID);
    }
    return ::utils::all_of(actionsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
}
}
}

#endif