#include <boost/format.hpp>

#include "SaveRawReport.h"
#include "db/db_info.h"
#include "db/factory/JsonReportFactory.h"

namespace db {
    namespace cmd {
        SaveRawReport::SaveRawReport(const db::RawReport &report) : report(report) {
        }
        
        void SaveRawReport::operator()(db::DatabaseSession &session) {
            session.execute(createInsert());
        }
        
        std::string SaveRawReport::createInsert() const {
            using namespace db::info::reports;
            
            boost::format insert("INSERT INTO %1%");
            boost::format into(" (%1%, %2%)");
            boost::format values(" VALUES (%1%, '%2%')");
            
            insert % tableName;
            into % colId % colDocument;
            values % report.ID % db::factory::JsonReportFactory::createJson(report);
            
            return insert.str() + into.str() + values.str();
        }
    }
}