#include <db/table/Counters.h>

#include "Counters.h"
#include "DefaultRepo.h"

namespace repository {

using Table = db::table::Counters;

namespace {
std::string toDbType(Counters::Type type);
}

Counters::Counters(db::DatabaseSession &session) : session(session) {
}

std::int32_t Counters::increment(Counters::Type type) {
    auto curVal = get(type);
    curVal++;
    set(type, curVal);
    return curVal;
}

std::int32_t Counters::get(Counters::Type type) {
    auto dbType = toDbType(type);
    auto sql = Table::Sql::select().where(Table::Name == dbType);
    auto row = session.getResult(sql).value();
    return std::get<Table::FieldCounter>(row).value;
}

void Counters::set(Counters::Type type, std::int32_t newVal) {
    auto dbType = toDbType(type);
    auto sql = Table::Sql::update().set(Table::Counter, newVal).where(Table::Name == dbType);
    session.execute(sql);
}

namespace {
std::string toDbType(Counters::Type type) {
    switch (type) {
        case Counters::Type::LastReportID:
            return "last_report_id";
        case Counters::Type::LastExhibitVersion:
            return "last_exhibit_version";
        case Counters::Type::LastMapVersion:
            return "last_map_version";
    }
}
}
}