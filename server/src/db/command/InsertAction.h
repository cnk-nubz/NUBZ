#ifndef DB_CMD__INSERT_ACTION__H
#define DB_CMD__INSERT_ACTION__H

#include <db/DatabaseSession.h>
#include <db/struct/Action.h>

namespace db {
namespace cmd {

class InsertAction {
public:
    InsertAction(const Action &action);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const Action action;
    std::int32_t actionId;

    std::string createInsert() const;
};
}
}

#endif