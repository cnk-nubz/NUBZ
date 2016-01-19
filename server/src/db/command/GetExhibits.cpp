#include <db/factory/ExhibitFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetExhibits.h"

namespace db {
namespace cmd {

GetExhibits::GetExhibits(std::int32_t exhibitId) : exhibitId(exhibitId) {
}

const std::vector<Exhibit> &GetExhibits::operator()(DatabaseSession &session) {
    return result = session.getResults<db::factory::ExhibitFactory>(createQuery());
}

const std::vector<Exhibit> &GetExhibits::getResult() const {
    return result;
}

std::string GetExhibits::createQuery() const {
    using namespace db::info::exhibits;
    using namespace db::sql;

    auto select = Sql::select(db::factory::ExhibitFactory::fieldsOrder()).from(tableName);

    if (minVersion) {
        select.where(C(colVersion) >= minVersion.value());
    }
    if (exhibitId) {
        select.where(C(colId) == exhibitId.value());
    }

    return select;
}
}
}