#include <db/db_info.h>
#include <db/sql.h>
#include <db/factory/SingleFieldFactory.h>

#include "InsertSortQuestionOption.h"

namespace db {
namespace cmd {

InsertSortQuestionOption::InsertSortQuestionOption(const SortQuestionOption &questionOption)
    : questionOption(questionOption) {
}

std::int32_t InsertSortQuestionOption::operator()(db::DatabaseSession &session) {
    return questionOptionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::int32_t InsertSortQuestionOption::getId() const {
    return questionOptionId;
}

std::string InsertSortQuestionOption::createInsert() const {
    using namespace db::info::sort_question_options;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colText, colQuestionId)
        .values(questionOption.text, questionOption.questionId)
        .returning(colId);
}
}
}