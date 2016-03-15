#include <repository/Experiments.h>

#include "StartExperiment.h"

namespace server {
namespace command {

StartExperiment::StartExperiment(db::Database &db) : db(db) {
}

void StartExperiment::operator()(std::int32_t ID) {
    db.execute([&](db::DatabaseSession &session) { repository::Experiments{session}.start(ID); });
}
}
}