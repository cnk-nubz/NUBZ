#ifndef SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H
#define SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H

#include <db/Database.h>

#include <server/io/input/RawReport.h>

#include "commons.h"

namespace server {
namespace command {

class SaveRawReportCommand {
public:
    SaveRawReportCommand(db::Database &db);
    SRV_CMD_CP_MV(SaveRawReportCommand);

    void operator()(const io::input::RawReport &input);

private:
    db::Database &db;

    io::input::RawReport removeDuplicatedIds(io::input::RawReport input) const;
    void validateReport(db::DatabaseSession &session, const io::input::RawReport &input) const;
};
}
}

#endif