#include <repository/Experiments.h>

#include "GetActiveExperiment.h"

namespace server {
namespace command {

GetActiveExperiment::GetActiveExperiment(db::Database &db) : db(db) {
}

io::output::SingleExperimentInfo GetActiveExperiment::operator()() {
    auto repoExp = db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Experiments{session};
        return repo.getLazyActive();
    });

    return io::output::SingleExperimentInfo{repoExp};
}
}
}