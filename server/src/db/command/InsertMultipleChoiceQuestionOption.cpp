#include <db/db_info.h>
#include <db/sql.h>
#include <db/factory/SingleFieldFactory.h>

#include "InsertMultipleChoiceQuestionOption.h"

namespace db {
namespace cmd {

InsertMultipleChoiceQuestionOption::InsertMultipleChoiceQuestionOption(
    const MultipleChoiceQuestionOption &questionOption)
    : questionOption(questionOption) {
}

std::int32_t InsertMultipleChoiceQuestionOption::operator()(db::DatabaseSession &session) {
    return questionOptionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::int32_t InsertMultipleChoiceQuestionOption::getId() const {
    return questionOptionId;
}

std::string InsertMultipleChoiceQuestionOption::createInsert() const {
    using namespace db::info::multiple_choice_question_options;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colText, colQuestionId)
        .values(questionOption.text, questionOption.questionId)
        .returning(colId);
}
}
}