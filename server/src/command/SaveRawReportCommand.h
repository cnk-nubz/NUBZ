#ifndef COMMAND__SAVE_RAW_REPORT_COMMAND__H
#define COMMAND__SAVE_RAW_REPORT_COMMAND__H

#include "commands_common.h"
#include "io/input/RawReport.h"
#include "db/Database.h"

namespace command {
    class SaveRawReportCommand {
    public:
        SaveRawReportCommand(db::Database &db);
        ~SaveRawReportCommand() = default;

        DEFAULT_CP_MV(SaveRawReportCommand);

        void operator()(const io::input::RawReport &input);

    private:
        db::Database &db;

        void validateReport(db::DatabaseSession &session, const io::input::RawReport &input) const;
    };
}

#endif