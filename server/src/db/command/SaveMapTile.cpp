#include <boost/format.hpp>

#include "db/db_info.h"
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

            boost::format insert("INSERT INTO %1%");
            boost::format into(" (%1%, %2%, %3%, %4%, %5%)");
            boost::format values(" VALUES ('%1%', %2%, %3%, %4%, %5%)");

            insert % tableName;
            into % colFilename % colFloor % colZoomLevel % colRow % colColumn;
            values % mapTile.filename % mapTile.floor % mapTile.zoomLevel % mapTile.row %
                mapTile.col;

            return insert.str() + into.str() + values.str();
        }
    }
}