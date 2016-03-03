#include "Counters.h"

namespace db {
namespace table {

const std::string Counters::tableName = "counters";
const std::string Counters::ColumnName::name = "name";
const std::string Counters::ColumnCounter::name = "counter";

const Counters::ColumnName Counters::colName{};
const Counters::ColumnCounter Counters::colCounter{};

Counters::Row Counters::RowFactory::fromDB(const DBOut &dbOut) {
    Counters::Row res;
    res.name = std::get<ValueName>(dbOut).value;
    res.counter = std::get<ValueCounter>(dbOut).value;
    return res;
}

Counters::RowFactory::DBIn Counters::RowFactory::toDB(const db::table::Counters::Row &row) {
    return std::make_tuple(ValueName{row.name}, ValueCounter{row.counter});
}
}
}