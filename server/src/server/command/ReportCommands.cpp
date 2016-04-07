#include <cstdlib>
#include <stdexcept>

#include <boost/format.hpp>

#include <utils/FileHandler.h>
#include <utils/log.h>

#include <repository/Counters.h>
#include <repository/Reports.h>

#include <server/utils/PathHelper.h>

#include "ReportCommands.h"

namespace server {
namespace command {

std::mutex ReportCommands::excelGenerationLock{};

ReportCommands::ReportCommands(db::Database &db) : db(db) {
}

void ReportCommands::save(const server::io::input::RawReport &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Reports{session};
        if (!repo.exist(input.ID)) {
            repo.insert(input.toRepo());
        }
    });
}

std::int32_t ReportCommands::reserveID() {
    return db.execute([](db::DatabaseSession &session) {
        auto repo = repository::Counters{session};
        return repo.increment(repository::CounterType::LastReportID);
    });
}

std::vector<ReportInfo> ReportCommands::getAllReportsForExperiment(std::int32_t experimentID) {
    auto repoInfos = db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Reports{session};
        return repo.getAllInfosForExperiment(experimentID);
    });

    return std::vector<ReportInfo>{repoInfos.begin(), repoInfos.end()};
}

std::string ReportCommands::getExcelReport(std::int32_t reportID) {
    std::int32_t experimentID;
    auto excelProc = db.execute([&](db::DatabaseSession &session) {
        experimentID = repository::Reports{session}.getF(reportID).experimentID;
        return ::utils::excel::ExcelProcessor::SingleReportProc(session, reportID);
    });

    auto path = utils::PathHelper::pathForExcelZip(experimentID, reportID);
    return createExcelReport(&excelProc, path);
}

std::string ReportCommands::getCombinedExcelReport(std::int32_t experimentID) {
    auto excelProc = db.execute([&](db::DatabaseSession &session) {
        return ::utils::excel::ExcelProcessor::AllReportsProc(session, experimentID);
    });

    auto path = utils::PathHelper::pathForExcelZip(experimentID);
    return createExcelReport(&excelProc, path);
}

std::string ReportCommands::createExcelReport(::utils::excel::ExcelProcessor *proc,
                                              const boost::filesystem::path &path) const {
    std::lock_guard<std::mutex> guard{excelGenerationLock};

    auto dataHandler = ::utils::FileHandler{utils::PathHelper::tmpDir.pathForFile("dane.csv")};
    auto legendHandler = ::utils::FileHandler{utils::PathHelper::tmpDir.pathForFile("legenda.csv")};
    auto zipHandler = ::utils::FileHandler{path.string() + ".zip"};

    LOG(INFO) << "Generating report";
    proc->saveReportToFile(dataHandler.getPath());

    LOG(INFO) << "Generating legend";
    proc->saveLegendToFile(legendHandler.getPath());

    LOG(INFO) << "Generating zip";

    // zip excepts filename without extension, extension will be added automatically
    zipFiles(dataHandler.getPath(), legendHandler.getPath(), path.string());
    if (!boost::filesystem::is_regular_file(zipHandler.getPath())) {
        throw std::runtime_error{"zip creation failed"};
    }

    LOG(INFO) << "Excel report ready, cleaning";
    zipHandler.release();
    return zipHandler.getPath().filename().string();
}

void ReportCommands::zipFiles(const boost::filesystem::path &src1,
                              const boost::filesystem::path &src2,
                              const boost::filesystem::path &dst) const {
    auto cmd = boost::format("zip --junk-paths %1% %2% %3%") % dst.string() % src1.string() %
               src2.string();
    auto cmdStr = cmd.str();
    LOG(INFO) << "Executing: " << cmdStr;
    system(cmdStr.c_str());
}
}
}