#include <repository/Counters.h>
#include <repository/Reports.h>

#include "ReportCommands.h"

namespace server {
namespace command {

ReportCommands::ReportCommands(db::Database &db) : db(db) {
}

void ReportCommands::save(const server::io::input::RawReport &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Reports{session};
        if (!repo.exist(input.ID)) {
            repo.insert(input.toRepo());
        }
    });
}

std::int32_t ReportCommands::reserveID() {
    return db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Counters{session};
        return repo.increment(repository::CounterType::LastReportID);
    });
}
}
}