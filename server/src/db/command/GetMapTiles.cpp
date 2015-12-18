#include <boost/format.hpp>

#include "GetMapTiles.h"
#include "db/factory/MapTileFactory.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        GetMapTiles::GetMapTiles(std::int32_t floor) : floor(floor) {
        }

        GetMapTiles::GetMapTiles(std::int32_t floor, std::int32_t zoomLevel)
            : floor(floor), zoomLevel(zoomLevel) {
        }

        void GetMapTiles::operator()(db::DatabaseSession &session) {
            result = session.getResults<db::factory::MapTileFactory>(createQuery());
        }

        const std::vector<MapTile> &GetMapTiles::getResult() const {
            return result;
        }

        std::string GetMapTiles::createQuery() const {
            using namespace db::info::map_tiles;

            boost::format select("SELECT %1%, %2%, %3%, %4%, %5%");
            boost::format from(" FROM %1%");

            for (const auto &col : db::factory::MapTileFactory::fieldsOrder()) {
                select % col;
            }
            from % tableName;

            std::string query = select.str() + from.str();

            if (floor && zoomLevel) {
                boost::format where(" WHERE %1% = %2% AND %3% = %4%");
                where % colFloor % floor.value();
                where % colZoomLevel % zoomLevel.value();
                query += where.str();
            } else if (floor) {
                boost::format where(" WHERE %1% = %2%");
                where % colFloor % floor.value();
                query += where.str();
            }

            return query;
        }
    }
}