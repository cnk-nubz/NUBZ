#include "SaveMapImage.h"
#include "db/db_info.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        SaveMapImage::SaveMapImage(const MapImage &mapImage) : mapImage(mapImage) {
        }

        void SaveMapImage::operator()(db::DatabaseSession &session) {
            if (!session.getResults(createQuery()).empty()) {
                session.execute(createUpdate());
            } else {
                session.execute(createInsert());
            }
        }

        std::string SaveMapImage::createQuery() const {
            using namespace db::info::map_images;
            using namespace db::sql;

            return Sql::select({"*"}).from(tableName).where(C(colFloor) == mapImage.floor);
        }

        std::string SaveMapImage::createInsert() const {
            using namespace db::info::map_images;
            using namespace db::sql;

            return Sql::insertInto(tableName)
                .what(colFilename, colWidth, colHeight, colFloor, colVersion)
                .values(mapImage.filename,
                        mapImage.width,
                        mapImage.height,
                        mapImage.floor,
                        mapImage.version);
        }

        std::string SaveMapImage::createUpdate() const {
            using namespace db::info::map_images;
            using namespace db::sql;

            return Sql::update(tableName)
                .where(C(colFloor) == mapImage.floor)
                .set(colFilename, mapImage.filename)
                .set(colVersion, mapImage.version)
                .set(colWidth, mapImage.width)
                .set(colHeight, mapImage.height)
                .set(colFloor, mapImage.floor);
        }
    }
}