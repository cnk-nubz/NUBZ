#include <algorithm>

#include <repository/Experiments.h>

#include "GetFinishedExperiments.h"

namespace server {
namespace command {

GetFinishedExperiments::GetFinishedExperiments(db::Database &db) : db(db) {
}

std::vector<io::output::ExperimentInfo> GetFinishedExperiments::operator()() {
    auto repoExperiments = db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        return repo.getAllFinished();
    });

    std::sort(repoExperiments.begin(), repoExperiments.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.startDate > rhs.startDate;
    });
    return std::vector<io::output::ExperimentInfo>(repoExperiments.begin(), repoExperiments.end());
}
}
}