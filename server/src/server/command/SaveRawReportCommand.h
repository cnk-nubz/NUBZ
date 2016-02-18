#ifndef SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H
#define SERVER_COMMAND__SAVE_RAW_REPORT_COMMAND__H

#include <server/utils/ReportChecker.h>

#include <db/Database.h>
#include <db/struct/Experiment.h>

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
    void validateAnswers(const utils::ReportChecker &reportChecker,
                         const io::input::RawReport::SurveyAnswers &answers,
                         const db::Experiment::Survey &survey) const;
};
}
}

#endif