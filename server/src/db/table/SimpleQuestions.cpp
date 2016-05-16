#include "SimpleQuestions.h"

namespace db {
namespace table {

const std::string SimpleQuestions::tableName = "simple_questions";

const std::string SimpleQuestions::FieldID::columnName = "id";
const std::string SimpleQuestions::FieldName::columnName = "name";
const std::string SimpleQuestions::FieldQuestion::columnName = "question";
const std::string SimpleQuestions::FieldNumberAnswer::columnName = "number_answer";
const std::string SimpleQuestions::FieldRefCount::columnName = "ref_count";
}
}