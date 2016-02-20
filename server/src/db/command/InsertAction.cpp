#include <db/db_info.h>
#include <db/sql.h>
#include <db/factory/SingleFieldFactory.h>

#include "InsertAction.h"

namespace db {
namespace cmd {

InsertAction::InsertAction(const Action &action) : action(action) {
}

std::int32_t InsertAction::operator()(db::DatabaseSession &session) {
    return actionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::string InsertAction::createInsert() const {
    using namespace db::info::actions;
    using namespace db::sql;

    return Sql::insertInto(tableName).what(colText).values(action.text).returning(colId);
}

std::int32_t InsertAction::getId() const {
    return actionId;
}
}
}