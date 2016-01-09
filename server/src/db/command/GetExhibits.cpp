#include "GetExhibits.h"
#include "db/db_info.h"
#include "db/factory/ExhibitFactory.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        GetExhibits::GetExhibits() : GetExhibits(0) {
        }

        GetExhibits::GetExhibits(std::int32_t minVersion) : minVersion(minVersion) {
        }

        void GetExhibits::operator()(DatabaseSession &session) {
            result = session.getResults<db::factory::ExhibitFactory>(createQuery());
        }

        const std::vector<Exhibit> &GetExhibits::getResult() const {
            return result;
        }

        std::string GetExhibits::createQuery() const {
            using namespace db::info::exhibits;
            using namespace db::sql;

            return Sql::select(db::factory::ExhibitFactory::fieldsOrder())
                .from(tableName)
                .where(C(colVersion) >= minVersion);
        }
    }
}