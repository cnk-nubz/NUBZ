#include <iostream>

#include <boost/format.hpp>

#include "SetCounter.h"
#include "db/db_info.h"
#include "db/factory/SingleFieldFactory.h"

namespace db {
    namespace cmd {
        SetCounter::SetCounter(db::info::counters::element_type elementType, std::int32_t newValue)
            : elementType(elementType), newValue(newValue) {
        }

        void SetCounter::operator()(DatabaseSession &session) {
            session.execute(createUpdate());
        }

        std::string SetCounter::createUpdate() const {
            using namespace db::info::counters;

            boost::format update("UPDATE %1%");
            boost::format set(" SET %1% = %2%");
            boost::format where(" WHERE %1% = '%2%'");

            update % tableName;
            set % colVersion % newValue;
            where % colElement % db::info::counters::colElementType(elementType);

            return update.str() + set.str() + where.str();
        }
    }
}