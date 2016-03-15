#include <repository/Counters.h>

#include "ReserveIdForReportCommand.h"

namespace server {
namespace command {

ReserveIdForReportCommand::ReserveIdForReportCommand(db::Database &db) : db(db) {
}

std::int32_t ReserveIdForReportCommand::operator()() {
    return db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Counters{session};
        return repo.increment(repository::CounterType::LastReportID);
    });
}
}
}