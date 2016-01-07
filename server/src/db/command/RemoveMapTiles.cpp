#include "RemoveMapTiles.h"
#include "db/db_info.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        RemoveMapTiles::RemoveMapTiles(std::int32_t floor) : floor(floor) {
        }

        void RemoveMapTiles::operator()(db::DatabaseSession &session) {
            session.execute(createDelete());
        }

        std::string RemoveMapTiles::createDelete() const {
            using namespace db::info::map_tiles;
            using namespace db::sql;

            return Sql::deleteFrom(tableName).where(C(colFloor) == floor);
        }
    }
}