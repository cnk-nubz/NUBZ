#include <db/command/IncrementCounter.h>

#include "ReserveIdForReportCommand.h"

namespace server {
namespace command {

ReserveIdForReportCommand::ReserveIdForReportCommand(db::Database &db) : db(db) {
}

std::int32_t ReserveIdForReportCommand::operator()() {
    auto incCmd = db::cmd::IncrementCounter::reportId();
    db.execute(incCmd);
    return incCmd.getNewVal();
}
}
}