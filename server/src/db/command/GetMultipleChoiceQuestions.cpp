#include <db/db_info.h>
#include <db/factory/MultipleChoiceQuestionFactory.h>
#include <db/sql.h>

#include "GetMultipleChoiceQuestions.h"

namespace db {
namespace cmd {

GetMultipleChoiceQuestions::GetMultipleChoiceQuestions(std::int32_t questionId)
    : questionId(questionId) {
}

std::vector<MultipleChoiceQuestion> GetMultipleChoiceQuestions::operator()(
    db::DatabaseSession &session) {
    return result = session.getResults<db::factory::MultipleChoiceQuestionFactory>(createQuery());
}

const std::vector<MultipleChoiceQuestion> &GetMultipleChoiceQuestions::getResult() const {
    return result;
}

std::string GetMultipleChoiceQuestions::createQuery() const {
    using namespace db::info::multiple_choice_questions;
    using namespace db::sql;

    auto select =
        Sql::select(db::factory::MultipleChoiceQuestionFactory::fieldsOrder()).from(tableName);
    if (questionId) {
        select.where(C(colId) == questionId.value());
    }
    return select;
}
}
}