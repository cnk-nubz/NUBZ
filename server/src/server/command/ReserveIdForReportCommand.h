#ifndef SERVER_COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H
#define SERVER_COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H

#include <db/Database.h>

#include "commons.h"

namespace server {
namespace command {

class ReserveIdForReportCommand {
public:
    ReserveIdForReportCommand(db::Database &db);
    SRV_CMD_CP_MV(ReserveIdForReportCommand);

    std::int32_t operator()();

private:
    db::Database &db;
};
}
}

#endif