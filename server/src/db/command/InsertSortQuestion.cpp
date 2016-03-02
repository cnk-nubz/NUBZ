#include <db/db_info.h>
#include <db/factory/SingleFieldFactory.h>
#include <db/sql.h>

#include "InsertSortQuestion.h"

namespace db {
namespace cmd {

InsertSortQuestion::InsertSortQuestion(const SortQuestion &question) : question(question) {
}

std::int32_t InsertSortQuestion::operator()(db::DatabaseSession &session) {
    return questionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::int32_t InsertSortQuestion::getId() const {
    return questionId;
}

std::string InsertSortQuestion::createInsert() const {
    using namespace db::info::sort_questions;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colName, colQuestion)
        .values(question.name, question.question)
        .returning(colId);
}
}
}