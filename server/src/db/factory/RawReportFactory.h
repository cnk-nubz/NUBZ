#ifndef DB_FACTORY__RAW_REPORT_FACTORY__H
#define DB_FACTORY__RAW_REPORT_FACTORY__H

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "db/struct/RawReport.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct RawReportFactory {
            using Product = RawReport;

            static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;
        };
    }
}

#endif