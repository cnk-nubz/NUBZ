#include <boost/format.hpp>

#include "GetRawReports.h"
#include "db/factory/RawReportFactory.h"

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

            boost::format select("SELECT %1%, %2%");
            boost::format from(" FROM %1%");

            select % colId % colDocument;
            from % tableName;

            std::string query = select.str() + from.str();

            if (reportId) {
                boost::format where(" WHERE %1%=%2%");
                where % colId % reportId.value();
                query += where.str();
            }

            return query;
        }
    }
}