#ifndef DB_CMD__GET_ACTIONS__H
#define DB_CMD__GET_ACTIONS__H

#include <db/struct/Action.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetActions {
public:
    const std::vector<Action> &operator()(DatabaseSession &session);
    const std::vector<Action> &getResult() const;

private:
    std::vector<db::Action> result;

    std::string createQuery() const;
};
}
}

#endif