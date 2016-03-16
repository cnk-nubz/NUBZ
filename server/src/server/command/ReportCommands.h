#ifndef SERVER_COMMAND__REPORT_COMMANDS__H
#define SERVER_COMMAND__REPORT_COMMANDS__H

#include <cstdint>

#include <db/Database.h>

#include <server/io/input/RawReport.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;

class ReportCommands : public Command {
public:
    ReportCommands(db::Database &db);

    void save(const RawReport &input);
    std::int32_t reserveID();

private:
    db::Database &db;
};
}
}

#endif