#include "SortQuestionOptions.h"

namespace db {
namespace table {

const std::string SortQuestionOptions::tableName = "sort_question_options";

const std::string SortQuestionOptions::FieldID::columnName = "id";
const std::string SortQuestionOptions::FieldQuestionID::columnName = "question_id";
const std::string SortQuestionOptions::FieldText::columnName = "text";
}
}