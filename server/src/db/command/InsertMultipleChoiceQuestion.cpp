#include <db/db_info.h>
#include <db/factory/SingleFieldFactory.h>
#include <db/sql.h>

#include "InsertMultipleChoiceQuestion.h"

namespace db {
namespace cmd {

InsertMultipleChoiceQuestion::InsertMultipleChoiceQuestion(const MultipleChoiceQuestion &question)
    : question(question) {
}

std::int32_t InsertMultipleChoiceQuestion::operator()(db::DatabaseSession &session) {
    return questionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::int32_t InsertMultipleChoiceQuestion::getId() const {
    return questionId;
}

std::string InsertMultipleChoiceQuestion::createInsert() const {
    using namespace db::info::multiple_choice_questions;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colName, colQuestion, colSingleAnswer)
        .values(question.name, question.question, question.singleAnswer)
        .returning(colId);
}
}
}