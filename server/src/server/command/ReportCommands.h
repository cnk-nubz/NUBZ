#ifndef SERVER_COMMAND__REPORT_COMMANDS__H
#define SERVER_COMMAND__REPORT_COMMANDS__H

#include <cstdint>
#include <mutex>

#include <utils/excel/ExcelProcessor.h>

#include <db/Database.h>

#include <server/io/input/RawReport.h>
#include <server/io/output/ReportInfo.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class ReportCommands : public Command {
public:
    ReportCommands(db::Database &db);

    void save(const RawReport &input);
    std::int32_t reserveID();

    std::vector<ReportInfo> getAllReportsForExperiment(std::int32_t experimentID);
    std::string getExcelReport(std::int32_t reportID);
    std::string getCombinedExcelReport(std::int32_t experimentID);

private:
    static std::mutex excelGenerationLock;

    std::string createExcelReport(::utils::excel::ExcelProcessor *proc,
                                  const boost::filesystem::path &path) const;
    void zipFiles(const boost::filesystem::path &src1, const boost::filesystem::path &src2,
                  const boost::filesystem::path &dst) const;

    db::Database &db;
};
}
}

#endif