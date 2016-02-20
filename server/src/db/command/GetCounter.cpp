#include <iostream>

#include <db/factory/SingleFieldFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetCounter.h"

namespace db {
namespace cmd {

GetCounter::GetCounter(db::info::counters::element_type elementType)
    : elementType(elementType), result(0) {
}

std::int32_t GetCounter::operator()(DatabaseSession &session) {
    return result = session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createQuery());
}

std::int32_t GetCounter::getResult() const {
    return result;
}

std::string GetCounter::createQuery() const {
    using namespace db::info::counters;
    using namespace db::sql;

    return Sql::select({colValue})
        .from(tableName)
        .where(C(colElement) == colElementType(elementType));
}
}
}