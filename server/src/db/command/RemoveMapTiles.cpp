#include <boost/format.hpp>

#include "RemoveMapTiles.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        RemoveMapTiles::RemoveMapTiles(std::int32_t floor) : floor(floor) {
        }

        void RemoveMapTiles::operator()(db::DatabaseSession &session) {
            session.execute(createDelete());
        }

        std::string RemoveMapTiles::createDelete() const {
            using namespace db::info::map_tiles;

            boost::format deleteFrom("DELETE FROM %1%");
            boost::format where(" WHERE %1% = %2%");

            deleteFrom % tableName;
            where % colFloor % floor;

            return deleteFrom.str() + where.str();
        }
    }
}