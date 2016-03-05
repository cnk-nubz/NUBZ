#include <db/factory/SortQuestionOptionFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetSortQuestionOptions.h"

namespace db {
namespace cmd {

GetSortQuestionOptions::GetSortQuestionOptions(std::int32_t questionId) : questionId(questionId) {
}

std::vector<SortQuestionOption> GetSortQuestionOptions::operator()(db::DatabaseSession &session) {
    return result = session.getResults<db::factory::SortQuestionOptionFactory>(createQuery());
}

const std::vector<SortQuestionOption> &GetSortQuestionOptions::getResult() const {
    return result;
}

std::string GetSortQuestionOptions::createQuery() const {
    using namespace db::info::sort_question_options;
    using namespace db::sql;

    auto select =
        Sql::select(db::factory::SortQuestionOptionFactory::fieldsOrder()).from(tableName);
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