#include <iostream>

#include <boost/format.hpp>

#include "GetMapImages.h"
#include "db/db_info.h"
#include "db/factory/MapImageFactory.h"

namespace db {
    namespace cmd {
        GetMapImages::GetMapImages() : GetMapImages(0) {
        }

        GetMapImages::GetMapImages(std::int32_t minVersion) : minVersion(minVersion) {
        }

        void GetMapImages::operator()(DatabaseSession &session) {
            result = session.getResults<db::factory::MapImageFactory>(createQuery());
        }

        const std::vector<MapImage> &GetMapImages::getResult() const {
            return result;
        }

        std::string GetMapImages::createQuery() const {
            using namespace db::info::map_images;

            boost::format select("SELECT %1%, %2%, %3%");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% %2% %3%");

            const auto &col = db::factory::MapImageFactory::fieldsOrder();
            select % col[0] % col[1] % col[2];
            from % tableName;
            where % colVersion % ">=" % minVersion;

            return select.str() + from.str() + where.str();
        }
    }
}