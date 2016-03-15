#include <algorithm>

#include <repository/Experiments.h>

#include "GetReadyExperiments.h"

namespace server {
namespace command {

GetReadyExperiments::GetReadyExperiments(db::Database &db) : db(db) {
}

std::vector<io::output::ExperimentInfo> GetReadyExperiments::operator()() {
    auto repoExperiments = db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        return repo.getAllReady();
    });

    std::sort(repoExperiments.begin(), repoExperiments.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name < rhs.name;
    });
    return std::vector<io::output::ExperimentInfo>(repoExperiments.begin(), repoExperiments.end());
}
}
}