#include <db/db_info.h>
#include <db/sql.h>
#include <db/factory/SingleFieldFactory.h>

#include "InsertSimpleQuestion.h"

namespace db {
namespace cmd {

InsertSimpleQuestion::InsertSimpleQuestion(const SimpleQuestion &sQuestion) : sQuestion(sQuestion) {
}

std::int32_t InsertSimpleQuestion::operator()(db::DatabaseSession &session) {
    return sQuestionId =
               session.getResult<db::factory::SingleFieldFactory<std::int32_t>>(createInsert());
}

std::int32_t InsertSimpleQuestion::getId() const {
    return sQuestionId;
}

std::string InsertSimpleQuestion::createInsert() const {
    using namespace db::info::simple_questions;
    using namespace db::sql;

    return Sql::insertInto(tableName)
        .what(colName, colQuestion, colNumberAnswer)
        .values(sQuestion.name, sQuestion.question, sQuestion.numberAnswer)
        .returning(colId);
}
}
}