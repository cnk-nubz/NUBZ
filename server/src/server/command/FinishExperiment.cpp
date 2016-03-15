#include <repository/Experiments.h>

#include "FinishExperiment.h"

namespace server {
namespace command {

FinishExperiment::FinishExperiment(db::Database &db) : db(db) {
}

void FinishExperiment::operator()() {
    db.execute(
        [](db::DatabaseSession &session) { repository::Experiments{session}.finishActive(); });
}
}
}