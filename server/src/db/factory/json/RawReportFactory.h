#ifndef DB_FACTORY_JSON__RAW_REPORT_FACTORY__H
#define DB_FACTORY_JSON__RAW_REPORT_FACTORY__H

#include <string>
#include <vector>

#include <db/db_info.h>
#include <db/struct/RawReport.h>

namespace db {
namespace factory {
namespace json {

struct RawReportFactory {
    using Product = RawReport;

    static std::string createJson(const Product &report);

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;
    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}
}

#endif