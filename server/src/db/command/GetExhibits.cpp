#include <string>

#include <boost/format.hpp>

#include "GetExhibits.h"
#include "db/db_info.h"
#include "db/factory/ExhibitFactory.h"

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

            boost::format select("SELECT %1%, %2%, %3%, %4%, %5%, %6%, %7%, %8%");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% %2% %3%");

            for (const auto &colName : db::factory::ExhibitFactory::fieldsOrder()) {
                select % colName;
            }
            from % tableName;
            where % colVersion % ">=" % minVersion;

            return select.str() + from.str() + where.str();
        }
    }
}