#include <db/db_info.h>
#include <db/factory/SortQuestionFactory.h>
#include <db/sql.h>

#include "GetSortQuestions.h"

namespace db {
namespace cmd {

GetSortQuestions::GetSortQuestions(std::int32_t questionId) : questionId(questionId) {
}

std::vector<SortQuestion> GetSortQuestions::operator()(db::DatabaseSession &session) {
    return result = session.getResults<db::factory::SortQuestionFactory>(createQuery());
}

const std::vector<SortQuestion> &GetSortQuestions::getResult() const {
    return result;
}

std::string GetSortQuestions::createQuery() const {
    using namespace db::info::sort_questions;
    using namespace db::sql;

    auto select = Sql::select(db::factory::SortQuestionFactory::fieldsOrder()).from(tableName);
    if (questionId) {
        select.where(C(colId) == questionId.value());
    }
    return select;
}
}
}