#ifndef DB_CMD__GET_ACTIONS__H
#define DB_CMD__GET_ACTIONS__H

#include <boost/optional.hpp>

#include <db/struct/Action.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetActions {
public:
    GetActions() = default;
    GetActions(std::int32_t actionId);

    const std::vector<Action> operator()(DatabaseSession &session);
    const std::vector<Action> &getResult() const;

    boost::optional<std::int32_t> actionId;

private:
    std::vector<db::Action> result;

    std::string createQuery() const;
};
}
}

#endif