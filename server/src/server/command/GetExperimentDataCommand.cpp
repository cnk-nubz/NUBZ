#include <db/command/GetActions.h>

#include <server/utils/io_translation.h>

#include "GetExperimentDataCommand.h"

namespace server {
namespace command {

GetExperimentDataCommand::GetExperimentDataCommand(db::Database &db) : db(db) {
}

io::output::ExperimentData GetExperimentDataCommand::operator()() {
    return io::output::ExperimentData{};
}
}
}