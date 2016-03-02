#ifndef REPOSITORY__ACTIONS__H
#define REPOSITORY__ACTIONS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

class Actions {
public:
    struct Action {
        std::int32_t ID;
        std::string text;
    };

    Actions(db::DatabaseSession &session);

    boost::optional<Action> get(std::int32_t ID);
    std::vector<Action> getAll();

    void remove(std::int32_t ID);
    void removeAll();

    // ID will be saved in the given struct
    void insert(Action *action);
    void insert(std::vector<Action> *actions);

private:
    db::DatabaseSession &session;
};

using Action = Actions::Action;
}

#endif