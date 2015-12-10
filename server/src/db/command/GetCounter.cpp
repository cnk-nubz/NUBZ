#include <iostream>

#include <boost/format.hpp>

#include "GetCounter.h"
#include "db/db_info.h"
#include "db/factory/SingleFieldFactory.h"

namespace db {
    namespace cmd {
        GetCounter::GetCounter(db::info::counters::element_type elementType)
            : elementType(elementType), result(0) {
        }

        void GetCounter::operator()(DatabaseSession &session) {
            result =
                session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createQuery());
        }

        std::int32_t GetCounter::getResult() const {
            return result;
        }

        std::string GetCounter::createQuery() const {
            using namespace db::info::counters;

            boost::format select("SELECT %1%");
            boost::format from(" FROM %1%");
            boost::format where(" WHERE %1% = '%2%'");

            select % colVersion;
            from % tableName;
            where % colElement % colElementType(elementType);

            return select.str() + from.str() + where.str();
        }
    }
}