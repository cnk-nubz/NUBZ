#include <db/db_info.h>
#include <db/sql.h>

#include "RemoveMapTilesInfo.h"

namespace db {
namespace cmd {

RemoveMapTilesInfo::RemoveMapTilesInfo(std::int32_t floor) : floor(floor) {
}

void RemoveMapTilesInfo::operator()(db::DatabaseSession &session) {
    session.execute(createDelete());
}

std::string RemoveMapTilesInfo::createDelete() const {
    using namespace db::info::map_tiles_info;
    using namespace db::sql;

    return Sql::deleteFrom(tableName).where(C(colFloor) == floor);
}
}
}