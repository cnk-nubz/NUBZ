#ifndef REPOSITORY__ACTIONS__H
#define REPOSITORY__ACTIONS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

/*
 * Validation:
 * Every method validates `ID`, `name`
 * and therefore can throw InvalidData or DuplicateName
 *
 * Deletion:
 * `remove` throws InUse if some experiment uses given action (reference count != 0)
 */
class Actions {
public:
    struct Action {
        std::int32_t ID;
        std::string text;
    };

    Actions(db::DatabaseSession &session);

    void incReferenceCount(std::int32_t ID);
    void decReferenceCount(std::int32_t ID);

    std::vector<std::int32_t> getAllIDs();

    // non-throwing get
    boost::optional<Action> getOpt(std::int32_t ID);

    Action get(std::int32_t ID);
    std::vector<Action> getAll();

    void remove(std::int32_t ID);

    void insert(Action *action);
    void insert(std::vector<Action> *actions);

private:
    void checkName(const std::string &name);

    db::DatabaseSession &session;
};

using Action = Actions::Action;
}

#endif