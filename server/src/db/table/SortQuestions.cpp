#include "SortQuestions.h"

namespace db {
namespace table {

const std::string SortQuestions::tableName = "sort_questions";

const std::string SortQuestions::FieldID::columnName = "id";
const std::string SortQuestions::FieldName::columnName = "name";
const std::string SortQuestions::FieldQuestion::columnName = "question";
const std::string SortQuestions::FieldRefCount::columnName = "ref_count";
}
}