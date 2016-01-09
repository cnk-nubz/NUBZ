#include "GetRawReports.h"
#include "db/factory/RawReportFactory.h"
#include "db/sql.h"

namespace db {
    namespace cmd {
        GetRawReports::GetRawReports(std::int32_t reportId) : reportId(reportId) {
        }

        void GetRawReports::operator()(DatabaseSession &session) {
            result = session.getResults<db::factory::RawReportFactory>(createQuery());
        }

        const std::vector<RawReport> &GetRawReports::getResult() const {
            return result;
        }

        std::string GetRawReports::createQuery() const {
            using namespace db::info::reports;
            using namespace db::sql;
            
            auto select = Sql::select(db::factory::RawReportFactory::fieldsOrder()).from(tableName);
            if (reportId) {
                select.where(C(colId) == reportId.value());
            }
            
            return select;
        }
    }
}