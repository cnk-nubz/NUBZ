#include "Actions.h"

namespace db {
namespace table {

const std::string Actions::tableName = "actions";
const std::string Actions::FieldID::columnName = "id";
const std::string Actions::FieldText::columnName = "text";
const std::string Actions::FieldRefCount::columnName = "ref_count";
}
}