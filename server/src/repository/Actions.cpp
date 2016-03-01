#include <utils/fp_algorithm.h>

#include <db/table/Actions.h>

#include "DefaultRepo.h"
#include "Actions.h"

namespace repository {

using Table = db::table::Actions;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
Table::Row toDB(const Actions::Action &action);
Actions::Action fromDB(const Table::Row &action);
}

Actions::Actions(db::DatabaseSession &session) : session(session) {
}

boost::optional<Actions::Action> Actions::get(std::int32_t ID) {
    if (auto res = Impl::get(session, ID)) {
        return fromDB(res.value());
    } else {
        return {};
    }
}

std::vector<Actions::Action> Actions::getAll() {
    std::vector<Actions::Action> result;
    utils::transform(Impl::getAll(session), result, fromDB);
    return result;
}

void Actions::remove(std::int32_t ID) {
    Impl::remove(session, ID);
}

void Actions::removeAll() {
    Impl::removeAll(session);
}

void Actions::insert(std::vector<Actions::Action> *actions) {
    assert(actions);
    for (auto &action : *actions) {
        insert(&action);
    }
}

void Actions::insert(Actions::Action *action) {
    assert(action);
    action->ID = Impl::insert(session, toDB(*action));
}

namespace {
Table::Row toDB(const Actions::Action &action) {
    Table::Row res;
    res.ID = action.ID;
    res.text = action.text;
    return res;
}

Actions::Action fromDB(const Table::Row &action) {
    Actions::Action res;
    res.ID = action.ID;
    res.text = action.text;
    return res;
}
}
}