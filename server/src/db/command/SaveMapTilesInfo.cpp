#include <boost/format.hpp>

#include "SaveMapTilesInfo.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        SaveMapTilesInfo::SaveMapTilesInfo(const MapTilesInfo &mapTileInfo)
            : mapTileInfo(mapTileInfo) {
        }

        void SaveMapTilesInfo::operator()(db::DatabaseSession &session) {
            session.execute(createInsert());
        }

        std::string SaveMapTilesInfo::createInsert() const {
            using namespace db::info::map_tiles_info;

            boost::format insert("INSERT INTO %1%");
            boost::format into(" (%1%, %2%, %3%, %4%, %5%, %6%, %7%)");
            boost::format values(" VALUES (%1%, %2%, %3%, %4%, %5%, %6%, %7%)");

            insert % tableName;
            into % colFloor % colZoomLevel % colRowsCount % colColumnsCount % colImgWidth %
                colImgHeight % colTileSize;
            values % mapTileInfo.floor % mapTileInfo.zoomLevel % mapTileInfo.rowsCount %
                mapTileInfo.colsCount % mapTileInfo.imgWidth % mapTileInfo.imgHeight %
                mapTileInfo.tileSize;

            return insert.str() + into.str() + values.str();
        }
    }
}