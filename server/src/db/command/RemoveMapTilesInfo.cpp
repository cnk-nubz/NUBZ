#include <boost/format.hpp>

#include "RemoveMapTilesInfo.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        RemoveMapTilesInfo::RemoveMapTilesInfo(std::int32_t floor) : floor(floor) {
        }

        void RemoveMapTilesInfo::operator()(db::DatabaseSession &session) {
            session.execute(createDelete());
        }

        std::string RemoveMapTilesInfo::createDelete() const {
            using namespace db::info::map_tiles_info;

            boost::format deleteFrom("DELETE FROM %1%");
            boost::format where(" WHERE %1% = %2%");

            deleteFrom % tableName;
            where % colFloor % floor;

            return deleteFrom.str() + where.str();
        }
    }
}