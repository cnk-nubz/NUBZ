#include <db/db_info.h>
#include <db/sql.h>

#include "SaveMapTile.h"

namespace db {
namespace cmd {

SaveMapTile::SaveMapTile(const MapTile &mapTile) : mapTile(mapTile) {
}

void SaveMapTile::operator()(db::DatabaseSession &session) {
    session.execute(createInsert());
}

std::string SaveMapTile::createInsert() const {
    using namespace db::info::map_tiles;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colFilename, colFloor, colZoomLevel, colRow, colColumn)
        .values(mapTile.filename, mapTile.floor, mapTile.zoomLevel, mapTile.row, mapTile.col);
}
}
}