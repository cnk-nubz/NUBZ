#include <boost/format.hpp>

#include "SaveMapImage.h"

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

            boost::format select("SELECT *");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% = %2%");

            from % tableName;
            where % colFloor % mapImage.floor;

            return select.str() + from.str() + where.str();
        }

        std::string SaveMapImage::createInsert() const {
            using namespace db::info::map_images;

            boost::format insert("INSERT INTO %1%");
            boost::format into(" (%1%, %2%, %3%, %4%, %5%)");
            boost::format values(" VALUES ('%1%', %2%, %3%, %4%, %5%)");

            insert % tableName;
            into % colFilename % colWidth % colHeight % colFloor % colVersion;
            values % mapImage.filename % mapImage.width % mapImage.height % mapImage.floor %
                mapImage.version;

            return insert.str() + into.str() + values.str();
        }

        std::string SaveMapImage::createUpdate() const {
            using namespace db::info::map_images;

            boost::format update("UPDATE %1%");
            boost::format set(" SET %1% = '%2%', %3% = %4%, %5% = %6%, %7% = %8%");
            boost::format where(" WHERE %1% = %2%");

            update % tableName;
            set % colFilename % mapImage.filename;
            set % colVersion % mapImage.version;
            set % colWidth % mapImage.width;
            set % colHeight % mapImage.height;
            where % colFloor % mapImage.floor;

            return update.str() + set.str() + where.str();
        }
    }
}