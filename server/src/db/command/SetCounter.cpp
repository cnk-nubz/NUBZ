#include <iostream>

#include <db/db_info.h>
#include <db/factory/SingleFieldFactory.h>
#include <db/sql.h>

#include "SetCounter.h"

namespace db {
namespace cmd {

SetCounter::SetCounter(db::info::counters::element_type elementType, std::int32_t newValue)
    : elementType(elementType), newValue(newValue) {
}

void SetCounter::operator()(DatabaseSession &session) {
    session.execute(createUpdate());
}

std::string SetCounter::createUpdate() const {
    using namespace db::info::counters;
    using namespace db::sql;

    return Sql::update(tableName)
        .set(colValue, newValue)
        .where(C(colElement) == colElementType(elementType));
}
}
}