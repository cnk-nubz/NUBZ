#include "SortQuestionOptions.h"

namespace db {
namespace table {

const std::string SortQuestionOptions::tableName = "sort_question_options";
const std::string SortQuestionOptions::ColumnId::name = "id";
const std::string SortQuestionOptions::ColumnQuestionId::name = "question_id";
const std::string SortQuestionOptions::ColumnText::name = "text";

const SortQuestionOptions::ColumnId SortQuestionOptions::colId{};
const SortQuestionOptions::ColumnQuestionId SortQuestionOptions::colQuestionId{};
const SortQuestionOptions::ColumnText SortQuestionOptions::colText{};

SortQuestionOptions::Row SortQuestionOptions::RowFactory::fromDB(const DBOut &dbOut) {
    SortQuestionOptions::Row res;
    res.ID = std::get<ValueId>(dbOut).value;
    res.questionID = std::get<ValueQuestionId>(dbOut).value;
    res.text = std::get<ValueText>(dbOut).value;
    return res;
}

SortQuestionOptions::RowFactory::DBIn SortQuestionOptions::RowFactory::toDB(
    const db::table::SortQuestionOptions::Row &row) {
    return std::make_tuple(ValueQuestionId{row.questionID}, ValueText{row.text});
}
}
}