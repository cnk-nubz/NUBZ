#include <db/factory/ActionFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetActions.h"

namespace db {
namespace cmd {

GetActions::GetActions(std::int32_t actionId) : actionId(actionId) {
}

std::vector<Action> GetActions::operator()(db::DatabaseSession &session) {
    return result = session.getResults<db::factory::ActionFactory>(createQuery());
}

const std::vector<Action> &GetActions::getResult() const {
    return result;
}

std::string GetActions::createQuery() const {
    using namespace db::info::actions;
    using namespace db::sql;

    auto select = Sql::select(db::factory::ActionFactory::fieldsOrder()).from(tableName);
    if (actionId) {
        select.where(C(colId) == actionId.value());
    }
    return select;
}
}
}