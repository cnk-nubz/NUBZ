#include <boost/format.hpp>

#include "GetMapTilesInfo.h"
#include "db/factory/MapTilesInfoFactory.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        GetMapTilesInfo::GetMapTilesInfo(std::int32_t floor) : floor(floor) {
        }

        void GetMapTilesInfo::operator()(db::DatabaseSession &session) {
            result = session.getResults<db::factory::MapTilesInfoFactory>(createQuery());
        }

        const std::vector<MapTilesInfo> &GetMapTilesInfo::getResult() const {
            return result;
        }

        std::string GetMapTilesInfo::createQuery() const {
            using namespace db::info::map_tiles_info;

            boost::format select("SELECT %1%, %2%, %3%, %4%, %5%, %6%, %7%");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% = %2%");

            for (const auto &col : db::factory::MapTilesInfoFactory::fieldsOrder()) {
                select % col;
            }
            from % tableName;
            where % colFloor % floor;

            return select.str() + from.str() + where.str();
        }
    }
}