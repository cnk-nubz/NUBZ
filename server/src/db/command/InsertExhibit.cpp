#include "InsertExhibit.h"
#include "db/db_info.h"
#include "db/sql.h"
#include "db/factory/SingleFieldFactory.h"

namespace db {
    namespace cmd {
        InsertExhibit::InsertExhibit(const Exhibit &exhibit) : exhibit(exhibit) {
        }

        void InsertExhibit::operator()(db::DatabaseSession &session) {
            exhibitId =
                session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
        }

        std::string InsertExhibit::createInsert() const {
            using namespace db::info::exhibits;
            using namespace db::sql;

            if (exhibit.frame) {
                auto frame = exhibit.frame.value();
                return Sql::insertInto(tableName)
                    .what(colName,
                          colVersion,
                          colMapLevel,
                          colMapFrameX,
                          colMapFrameY,
                          colMapFrameWidth,
                          colMapFrameHeight)
                    .values(exhibit.name,
                            exhibit.version,
                            frame.mapLevel,
                            frame.x,
                            frame.y,
                            frame.width,
                            frame.height)
                    .returning(colId);
            } else {
                return Sql::insertInto(tableName)
                    .what(colName, colVersion)
                    .values(exhibit.name, exhibit.version)
                    .returning(colId);
            }
        }

        std::int32_t InsertExhibit::getId() const {
            return exhibitId;
        }
    }
}