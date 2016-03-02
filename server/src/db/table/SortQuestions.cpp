#include "SortQuestions.h"

namespace db {
namespace table {

const std::string SortQuestions::tableName = "sort_questions";
const std::string SortQuestions::ColumnId::name = "id";
const std::string SortQuestions::ColumnName::name = "name";
const std::string SortQuestions::ColumnQuestion::name = "question";

const SortQuestions::ColumnId SortQuestions::colId{};
const SortQuestions::ColumnName SortQuestions::colName{};
const SortQuestions::ColumnQuestion SortQuestions::colQuestion{};

SortQuestions::Row SortQuestions::RowFactory::fromDB(const DBOut &dbOut) {
    SortQuestions::Row res;
    res.ID = std::get<ValueId>(dbOut).value;
    res.name = std::get<ValueName>(dbOut).value;
    res.question = std::get<ValueQuestion>(dbOut).value;
    return res;
}

SortQuestions::RowFactory::DBIn SortQuestions::RowFactory::toDB(
    const db::table::SortQuestions::Row &row) {
    return std::make_tuple(ValueName{row.name}, ValueQuestion{row.question});
}
}
}