#include <iostream>

#include <boost/format.hpp>
#include <pqxx/pqxx>

#include "GetVersion.h"
#include "db/db_info.h"
#include "db/factory/SingleFieldFactory.h"

namespace db {
    namespace cmd {
        GetVersion::GetVersion(ElementType elementType) : elementType(elementType), result(0) {
        }

        void GetVersion::perform(DatabaseSession &session) {
            result =
                session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createQuery());
        }

        std::int32_t GetVersion::getResult() const {
            return result;
        }

        std::string GetVersion::createQuery() const {
            using namespace db::info::versions;

            boost::format select("SELECT %1%");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% = '%2%'");

            select % colVersion;
            from % tableName;
            where % colElement % elementTypeName();

            return select.str() + from.str() + where.str();
        }

        std::string GetVersion::elementTypeName() const {
            using namespace db::info::versions;
            switch (elementType) {
                case ElementType::MapImage:
                    return element_option::map_image;
            }
        }
    }
}