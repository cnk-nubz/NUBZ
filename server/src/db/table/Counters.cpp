#include "Counters.h"

namespace db {
namespace table {

const std::string Counters::tableName = "counters";
const std::string Counters::FieldName::columnName = "name";
const std::string Counters::FieldCounter::columnName = "counter";
}
}