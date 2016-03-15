#ifndef SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H
#define SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H

#include <db/Database.h>

#include <server/io/input/RawReport.h>

#include "Command.h"

namespace server {
namespace command {

class SaveRawReportCommand : public Command {
public:
    SaveRawReportCommand(db::Database &db);

    void operator()(const io::input::RawReport &input);

private:
    db::Database &db;
};
}
}

#endif