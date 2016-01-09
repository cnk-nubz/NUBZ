#include <string>

#include "GetMapImages.h"
#include "db/db_info.h"
#include "db/factory/MapImageFactory.h"
#include "db/sql.h"

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
            using namespace db::sql;

            return Sql::select(db::factory::MapImageFactory::fieldsOrder())
                .from(tableName)
                .where(C(colVersion) >= minVersion);
        }
    }
}