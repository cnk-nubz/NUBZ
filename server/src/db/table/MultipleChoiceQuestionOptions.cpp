#include "MultipleChoiceQuestionOptions.h"

namespace db {
namespace table {

const std::string MultipleChoiceQuestionOptions::tableName = "multiple_choice_question_options";

const std::string MultipleChoiceQuestionOptions::FieldID::columnName = "id";
const std::string MultipleChoiceQuestionOptions::FieldQuestionID::columnName = "question_id";
const std::string MultipleChoiceQuestionOptions::FieldText::columnName = "text";
}
}