#include "SimpleQuestions.h"

namespace db {
namespace table {

const std::string SimpleQuestions::tableName = "simple_questions";
const std::string SimpleQuestions::ColumnId::name = "id";
const std::string SimpleQuestions::ColumnName::name = "name";
const std::string SimpleQuestions::ColumnQuestion::name = "question";
const std::string SimpleQuestions::ColumnNumberAnswer::name = "number_answer";

const SimpleQuestions::ColumnId SimpleQuestions::colId{};
const SimpleQuestions::ColumnName SimpleQuestions::colName{};
const SimpleQuestions::ColumnQuestion SimpleQuestions::colQuestion{};
const SimpleQuestions::ColumnNumberAnswer SimpleQuestions::colNumberAnswer{};

SimpleQuestions::Row SimpleQuestions::RowFactory::fromDB(const DBOut &dbOut) {
    SimpleQuestions::Row res;
    res.ID = std::get<ValueId>(dbOut).value;
    res.name = std::get<ValueName>(dbOut).value;
    res.question = std::get<ValueQuestion>(dbOut).value;
    res.numberAnswer = std::get<ValueNumberAnswer>(dbOut).value;
    return res;
}

SimpleQuestions::RowFactory::DBIn SimpleQuestions::RowFactory::toDB(
    const db::table::SimpleQuestions::Row &row) {
    return std::make_tuple(
        ValueName{row.name}, ValueQuestion{row.question}, ValueNumberAnswer{row.numberAnswer});
}
}
}