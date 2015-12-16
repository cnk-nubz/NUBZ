#include "ReserveIdForReportCommand.h"
#include "db/command/GetCounter.h"
#include "db/command/SetCounter.h"

namespace command {
    ReserveIdForReportCommand::ReserveIdForReportCommand(db::Database &db) : db(db) {
    }

    std::int32_t ReserveIdForReportCommand::operator()() {
        std::int32_t reservedId;
        db.execute([&](db::DatabaseSession &session) {
            auto counterType = db::info::counters::element_type::reports;

            db::cmd::GetCounter get(counterType);
            get(session);
            reservedId = get.getResult() + 1;

            db::cmd::SetCounter{counterType, reservedId}(session);
        });

        return reservedId;
    }
}