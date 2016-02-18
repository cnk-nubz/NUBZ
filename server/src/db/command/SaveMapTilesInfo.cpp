#include <db/db_info.h>
#include <db/sql.h>

#include "SaveMapTilesInfo.h"

namespace db {
namespace cmd {

SaveMapTilesInfo::SaveMapTilesInfo(const MapTilesInfo &mapTileInfo) : mapTileInfo(mapTileInfo) {
}

void SaveMapTilesInfo::operator()(db::DatabaseSession &session) {
    session.execute(createInsert());
}

std::string SaveMapTilesInfo::createInsert() const {
    using namespace db::info::map_tiles_info;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colFloor,
              colZoomLevel,
              colRowsCount,
              colColumnsCount,
              colImgWidth,
              colImgHeight,
              colTileSize)
        .values(mapTileInfo.floor,
                mapTileInfo.zoomLevel,
                mapTileInfo.rowsCount,
                mapTileInfo.colsCount,
                mapTileInfo.imgWidth,
                mapTileInfo.imgHeight,
                mapTileInfo.tileSize);
}
}
}