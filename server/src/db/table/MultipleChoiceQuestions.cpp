#include "MultipleChoiceQuestions.h"

namespace db {
namespace table {

const std::string MultipleChoiceQuestions::tableName = "multiple_choice_questions";
const std::string MultipleChoiceQuestions::ColumnId::name = "id";
const std::string MultipleChoiceQuestions::ColumnName::name = "name";
const std::string MultipleChoiceQuestions::ColumnQuestion::name = "question";
const std::string MultipleChoiceQuestions::ColumnSingleAnswer::name = "single_answer";

const MultipleChoiceQuestions::ColumnId MultipleChoiceQuestions::colId{};
const MultipleChoiceQuestions::ColumnName MultipleChoiceQuestions::colName{};
const MultipleChoiceQuestions::ColumnQuestion MultipleChoiceQuestions::colQuestion{};
const MultipleChoiceQuestions::ColumnSingleAnswer MultipleChoiceQuestions::colSingleAnswer{};

MultipleChoiceQuestions::Row MultipleChoiceQuestions::RowFactory::fromDB(const DBOut &dbOut) {
    auto res = MultipleChoiceQuestions::Row{};
    res.ID = std::get<ValueId>(dbOut).value;
    res.name = std::get<ValueName>(dbOut).value;
    res.question = std::get<ValueQuestion>(dbOut).value;
    res.singleAnswer = std::get<ValueSingleAnswer>(dbOut).value;
    return res;
}

MultipleChoiceQuestions::RowFactory::DBIn MultipleChoiceQuestions::RowFactory::toDB(
    const db::table::MultipleChoiceQuestions::Row &row) {
    return std::make_tuple(
        ValueName{row.name}, ValueQuestion{row.question}, ValueSingleAnswer{row.singleAnswer});
}
}
}