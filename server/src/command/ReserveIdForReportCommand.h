#ifndef COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H
#define COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H

#include "commands_common.h"
#include "db/Database.h"

namespace command {
    class ReserveIdForReportCommand {
    public:
        ReserveIdForReportCommand(db::Database &db);
        ~ReserveIdForReportCommand() = default;

        DEFAULT_CP_MV(ReserveIdForReportCommand);

        std::int32_t operator()();

    private:
        db::Database &db;
    };
}

#endif