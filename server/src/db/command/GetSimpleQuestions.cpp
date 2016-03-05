#include <db/factory/SimpleQuestionFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetSimpleQuestions.h"

namespace db {
namespace cmd {

GetSimpleQuestions::GetSimpleQuestions(std::int32_t simpleQId) : simpleQuestionId(simpleQId) {
}

std::vector<SimpleQuestion> GetSimpleQuestions::operator()(DatabaseSession &session) {
    return result = session.getResults<db::factory::SimpleQuestionFactory>(createQuery());
}

const std::vector<SimpleQuestion> &GetSimpleQuestions::getResult() const {
    return result;
}

std::string GetSimpleQuestions::createQuery() const {
    using namespace db::info::simple_questions;
    using namespace db::sql;

    auto select = Sql::select(db::factory::SimpleQuestionFactory::fieldsOrder()).from(tableName);

    if (simpleQuestionId) {
        select.where(C(colId) == simpleQuestionId.value());
    }

    return select;
}
}
}