#include <repository/Experiments.h>

#include <server/io/InvalidInput.h>

#include "GetExperiment.h"

namespace server {
namespace command {

GetExperiment::GetExperiment(db::Database &db) : db(db) {
}

io::output::Experiment GetExperiment::operator()(std::int32_t ID) {
    auto repoExpr = db.execute([&](db::DatabaseSession &session) {
        if (auto exp = repository::Experiments{session}.get(ID)) {
            return exp.value();
        } else {
            throw io::InvalidInput{"experiment with given id doesn't exist"};
        }
    });

    return io::output::Experiment{repoExpr};
}
}
}