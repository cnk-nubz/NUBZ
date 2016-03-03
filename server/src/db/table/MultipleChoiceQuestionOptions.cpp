#include "MultipleChoiceQuestionOptions.h"

namespace db {
namespace table {

const std::string MultipleChoiceQuestionOptions::tableName = "multiple_choice_question_options";
const std::string MultipleChoiceQuestionOptions::ColumnId::name = "id";
const std::string MultipleChoiceQuestionOptions::ColumnQuestionId::name = "question_id";
const std::string MultipleChoiceQuestionOptions::ColumnText::name = "text";

const MultipleChoiceQuestionOptions::ColumnId MultipleChoiceQuestionOptions::colId{};
const MultipleChoiceQuestionOptions::ColumnQuestionId
    MultipleChoiceQuestionOptions::colQuestionId{};
const MultipleChoiceQuestionOptions::ColumnText MultipleChoiceQuestionOptions::colText{};

MultipleChoiceQuestionOptions::Row MultipleChoiceQuestionOptions::RowFactory::fromDB(
    const DBOut &dbOut) {
    auto res = MultipleChoiceQuestionOptions::Row{};
    res.ID = std::get<ValueId>(dbOut).value;
    res.questionID = std::get<ValueQuestionId>(dbOut).value;
    res.text = std::get<ValueText>(dbOut).value;
    return res;
}

MultipleChoiceQuestionOptions::RowFactory::DBIn MultipleChoiceQuestionOptions::RowFactory::toDB(
    const db::table::MultipleChoiceQuestionOptions::Row &row) {
    return std::make_tuple(ValueQuestionId{row.questionID}, ValueText{row.text});
}
}
}