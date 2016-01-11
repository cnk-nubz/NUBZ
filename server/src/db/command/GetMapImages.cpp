#include <string>

#include "GetMapImages.h"
#include "db/db_info.h"
#include "db/factory/MapImageFactory.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        GetMapImages::GetMapImages() {
        }

        GetMapImages::GetMapImages(std::int32_t floor) : floor(floor) {
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

            auto select = Sql::select(db::factory::MapImageFactory::fieldsOrder()).from(tableName);

            if (minVersion) {
                select.where(C(colVersion) >= minVersion.value());
            }
            if (floor) {
                select.where(C(colFloor) >= floor.value());
            }

            return select;
        }
    }
}