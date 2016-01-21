#include <db/db_info.h>
#include <db/sql.h>

#include "SetCurrentExperiment.h"

namespace db {
namespace cmd {

SetCurrentExperiment::SetCurrentExperiment(std::int32_t newId) : newId(newId) {
}

void SetCurrentExperiment::operator()(db::DatabaseSession &session) {
    session.execute(createUpdate());
}

std::string SetCurrentExperiment::createUpdate() const {
    using namespace db::info::experiments::current;
    using namespace db::sql;

    return Sql::update(tableName).set(colId, newId);
}
}
}