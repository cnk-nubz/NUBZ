#include "MultipleChoiceQuestions.h"

namespace db {
namespace table {

const std::string MultipleChoiceQuestions::tableName = "multiple_choice_questions";

const std::string MultipleChoiceQuestions::FieldID::columnName = "id";
const std::string MultipleChoiceQuestions::FieldName::columnName = "name";
const std::string MultipleChoiceQuestions::FieldQuestion::columnName = "question";
const std::string MultipleChoiceQuestions::FieldSingleAnswer::columnName = "single_answer";
}
}