#include <db/factory/json/RawReportFactory.h>
#include <db/sql.h>

#include "GetRawReports.h"

namespace db {
namespace cmd {

GetRawReports::GetRawReports(std::int32_t reportId) : reportId(reportId) {
}

const std::vector<RawReport> &GetRawReports::operator()(DatabaseSession &session) {
    return result = session.getResults<db::factory::json::RawReportFactory>(createQuery());
}

const std::vector<RawReport> &GetRawReports::getResult() const {
    return result;
}

std::string GetRawReports::createQuery() const {
    using namespace db::info::reports;
    using namespace db::sql;

    auto select = Sql::select(db::factory::json::RawReportFactory::fieldsOrder()).from(tableName);
    if (reportId) {
        select.where(C(colId) == reportId.value());
    }

    return select;
}
}
}