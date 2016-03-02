#include <db/db_info.h>
#include <db/factory/SingleFieldFactory.h>
#include <db/sql.h>

#include "GetCurrentExperiment.h"
#include "GetExperiments.h"

namespace db {
namespace cmd {

boost::optional<Experiment> GetCurrentExperiment::operator()(DatabaseSession &session) {
    boost::optional<std::string> currentIdStr = session.getResult(createQuery()).front();
    if (!currentIdStr) {
        return result;
    }

    std::int32_t currentId = db::factory::SingleFieldFactory<std::int32_t>::create({currentIdStr});
    return result = GetExperiments{currentId}(session).front();
}

const boost::optional<Experiment> &GetCurrentExperiment::getResult() const {
    return result;
}

std::string GetCurrentExperiment::createQuery() const {
    using namespace db::info::experiments::current;
    using namespace db::sql;

    return Sql::select({colId}).from(tableName);
}
}
}