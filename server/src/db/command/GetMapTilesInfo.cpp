#include <db/factory/MapTilesInfoFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetMapTilesInfo.h"

namespace db {
namespace cmd {

GetMapTilesInfo::GetMapTilesInfo(std::int32_t floor) : floor(floor) {
}

const std::vector<MapTilesInfo> GetMapTilesInfo::operator()(db::DatabaseSession &session) {
    return result = session.getResults<db::factory::MapTilesInfoFactory>(createQuery());
}

const std::vector<MapTilesInfo> &GetMapTilesInfo::getResult() const {
    return result;
}

std::string GetMapTilesInfo::createQuery() const {
    using namespace db::info::map_tiles_info;
    using namespace db::sql;

    return Sql::select(db::factory::MapTilesInfoFactory::fieldsOrder())
        .from(tableName)
        .where(C(colFloor) == floor);
}
}
}