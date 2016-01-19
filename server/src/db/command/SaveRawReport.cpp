#include <db/factory/json/RawReportFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "SaveRawReport.h"

namespace db {
namespace cmd {

SaveRawReport::SaveRawReport(const db::RawReport &report) : report(report) {
}

void SaveRawReport::operator()(db::DatabaseSession &session) {
    session.execute(createInsert());
}

std::string SaveRawReport::createInsert() const {
    using namespace db::info::reports;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colId, colDocument)
        .values(report.ID, db::factory::json::RawReportFactory::createJson(report));
}
}
}