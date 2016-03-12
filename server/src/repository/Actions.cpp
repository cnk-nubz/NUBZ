#include <utils/fp_algorithm.h>

#include <db/table/Actions.h>

#include "Actions.h"
#include "DefaultRepo.h"

namespace repository {

using Table = db::table::Actions;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
Table::Sql::in_t toDB(const Actions::Action &action);
Actions::Action fromDB(const Table::Sql::out_t &actionTuple);
}

Actions::Actions(db::DatabaseSession &session) : session(session) {
}

std::vector<std::int32_t> Actions::getAllIDs() {
    return Impl::getAllIDs(session);
}

boost::optional<Actions::Action> Actions::get(std::int32_t ID) {
    if (auto res = Impl::get(session, ID)) {
        return fromDB(res.value());
    } else {
        return {};
    }
}

std::vector<Actions::Action> Actions::getAll() {
    auto result = std::vector<Actions::Action>{};
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
Table::Sql::in_t toDB(const Actions::Action &action) {
    return std::make_tuple(Table::FieldText{action.text});
}

Actions::Action fromDB(const Table::Sql::out_t &actionTuple) {
    auto res = Actions::Action{};
    res.ID = std::get<Table::FieldID>(actionTuple).value;
    res.text = std::get<Table::FieldText>(actionTuple).value;
    return res;
}
}
}