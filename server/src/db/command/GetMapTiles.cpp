#include "GetMapTiles.h"
#include "db/factory/MapTileFactory.h"
#include "db/db_info.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        void GetMapTiles::operator()(db::DatabaseSession &session) {
            result = session.getResults<db::factory::MapTileFactory>(createQuery());
        }

        const std::vector<MapTile> &GetMapTiles::getResult() const {
            return result;
        }

        std::string GetMapTiles::createQuery() const {
            using namespace db::info::map_tiles;
            using namespace db::sql;

            auto select = Sql::select(db::factory::MapTileFactory::fieldsOrder()).from(tableName);
            if (floor) {
                select.where(C(colFloor) == floor.value());
            }
            if (zoomLevel) {
                select.where(C(colZoomLevel) == zoomLevel.value());
            }
            return select;
        }
    }
}