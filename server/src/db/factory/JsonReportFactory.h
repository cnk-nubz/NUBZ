#ifndef DB_FACTORY__JSON_REPORT_FACTORY__H
#define DB_FACTORY__JSON_REPORT_FACTORY__H

#include <string>

#include "db/db_info.h"
#include "db/struct/RawReport.h"

namespace db {
    namespace factory {
        struct JsonReportFactory {
            static std::string createJson(const db::RawReport &report);
        };
    }
}

#endif