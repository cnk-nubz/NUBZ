#include <db/db_info.h>
#include <db/factory/MultipleChoiceQuestionOptionFactory.h>
#include <db/sql.h>

#include "GetMultipleChoiceQuestionOptions.h"

namespace db {
namespace cmd {

GetMultipleChoiceQuestionOptions::GetMultipleChoiceQuestionOptions(std::int32_t questionId)
    : questionId(questionId) {
}

std::vector<MultipleChoiceQuestionOption> GetMultipleChoiceQuestionOptions::operator()(
    db::DatabaseSession &session) {
    return result =
               session.getResults<db::factory::MultipleChoiceQuestionOptionFactory>(createQuery());
}

const std::vector<MultipleChoiceQuestionOption> &GetMultipleChoiceQuestionOptions::getResult()
    const {
    return result;
}

std::string GetMultipleChoiceQuestionOptions::createQuery() const {
    using namespace db::info::multiple_choice_question_options;
    using namespace db::sql;

    auto select = Sql::select(db::factory::MultipleChoiceQuestionOptionFactory::fieldsOrder())
                      .from(tableName);
    if (questionId) {
        select.where(C(colQuestionId) == questionId.value());
    }
    if (optionId) {
        select.where(C(colId) == optionId.value());
    }
    return select;
}
}
}