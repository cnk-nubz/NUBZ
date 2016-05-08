#include <utils/fp_algorithm.h>

#include <db/table/Actions.h>

#include "Actions.h"
#include "DefaultRepo.h"
#include "error/DuplicateName.h"
#include "error/InUse.h"
#include "error/InvalidData.h"

namespace repository {

using Table = db::table::Actions;
using Impl = repository::detail::DefaultRepoIDRefCount<Table>;

namespace {
Table::Sql::in_t toDB(const Actions::Action &action);
Actions::Action fromDB(const Table::Sql::out_t &actionTuple);
}

Actions::Actions(db::DatabaseSession &session) : session(session) {
}

void Actions::incReferenceCount(std::int32_t ID) {
    Impl::incRefCount(session, ID);
}

void Actions::decReferenceCount(std::int32_t ID) {
    Impl::decRefCount(session, ID);
}

std::vector<std::int32_t> Actions::getAllIDs() {
    return Impl::getAllIDs(session);
}

Actions::Action Actions::get(std::int32_t ID) {
    if (auto res = getOpt(ID)) {
        return res.value();
    } else {
        throw InvalidData{"incorrect action ID"};
    }
}

boost::optional<Actions::Action> Actions::getOpt(std::int32_t ID) {
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
    auto sql = Table::Sql::select().where(Table::ID == ID);
    auto dbTuple = session.getResult(sql);

    if (!dbTuple) {
        throw InvalidData{"incorrect action ID"};
    }
    if (std::get<Table::FieldRefCount>(dbTuple.value()).value != 0) {
        throw InUse{"action in use"};
    }
    Impl::remove(session, ID);
}

void Actions::insert(std::vector<Actions::Action> *actions) {
    assert(actions);
    for (auto &action : *actions) {
        insert(&action);
    }
}

void Actions::insert(Actions::Action *action) {
    assert(action);
    checkName(action->text);
    action->ID = Impl::insert(session, toDB(*action));
}

void Actions::checkName(const std::string &name) {
    if (name.empty()) {
        throw InvalidData{"action text cannot be empty"};
    }

    auto sql = db::sql::Select<Table::FieldText>{}.where(Table::Text == name);
    if (session.getResult(sql)) {
        throw DuplicateName{};
    }
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