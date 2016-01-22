#include <db/factory/json/ExperimentFactory.h>
#include <db/db_info.h>
#include <db/sql.h>

#include "GetExperiments.h"

namespace db {
namespace cmd {

GetExperiments::GetExperiments(std::int32_t experimentId) : experimentId(experimentId) {
}

const std::vector<Experiment> GetExperiments::operator()(db::DatabaseSession &session) {
    return result = session.getResults<db::factory::json::ExperimentFactory>(createQuery());
}

const std::vector<Experiment> &GetExperiments::getResult() const {
    return result;
}

std::string GetExperiments::createQuery() const {
    using namespace db::info::experiments;
    using namespace db::sql;

    auto select = Sql::select(db::factory::json::ExperimentFactory::fieldsOrder()).from(tableName);
    if (experimentId) {
        select.where(C(colId) == experimentId.value());
    }
    return select;
}
}
}