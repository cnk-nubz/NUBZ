#ifndef SERVER_COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H
#define SERVER_COMMAND__RESERVE_ID_FOR_REPORT_COMMAND__H

#include <db/Database.h>

#include "Command.h"

namespace server {
namespace command {

class ReserveIdForReportCommand : public Command {
public:
    ReserveIdForReportCommand(db::Database &db);

    std::int32_t operator()();

private:
    db::Database &db;
};
}
}

#endif