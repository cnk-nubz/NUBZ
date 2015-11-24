#include <iostream>

#include <boost/format.hpp>

#include "SetVersion.h"
#include "db/db_info.h"
#include "db/factory/SingleFieldFactory.h"

namespace db {
    namespace cmd {
        SetVersion::SetVersion(db::info::versions::element_type elementType,
                               std::int32_t newVersion)
            : elementType(elementType), newVersion(newVersion) {
        }

        void SetVersion::operator()(DatabaseSession &session) {
            session.execute(createUpdate());
        }

        std::string SetVersion::createUpdate() const {
            using namespace db::info::versions;

            boost::format update("UPDATE %1%");
            boost::format set(" SET %1% = %2%");
            boost::format where(" WHERE %1% = '%2%'");

            update % tableName;
            set % colVersion % newVersion;
            where % colElement % db::info::versions::colElementType(elementType);

            return update.str() + set.str() + where.str();
        }
    }
}