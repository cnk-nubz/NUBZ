#ifndef UTILS_EXCEL__EXCEL_PROCESSOR__H
#define UTILS_EXCEL__EXCEL_PROCESSOR__H

#include <fstream>

#include <boost/filesystem.hpp>

#include <utils/FileHandler.h>

#include <db/DatabaseSession.h>

#include <repository/Exhibits.h>
#include <repository/Experiments.h>
#include <repository/Reports.h>

namespace utils {
namespace excel {

class ExcelProcessor {
public:
    // Session is used only for creation
    static ExcelProcessor SingleReportProc(db::DatabaseSession &session, std::int32_t reportID);
    static ExcelProcessor AllReportsProc(db::DatabaseSession &session, std::int32_t experimentID);

    // You probably don't want to do this inside db transaction
    void saveReportToFile(const boost::filesystem::path &output) const;
    void saveLegendToFile(const boost::filesystem::path &output) const;

private:
    ExcelProcessor(db::DatabaseSession &session, const repository::Experiment &experiment,
                   const std::vector<repository::Report> &reports);

    template <class Func>
    void safeSaving(const boost::filesystem::path &output, Func &&func) const {
        auto fileHandler = ::utils::FileHandler{output};
        auto outputStream = std::ofstream{};
        outputStream.open(output.string());

        func(outputStream);

        outputStream.close();
        fileHandler.release();
    }

    void saveReport(std::ofstream &output) const;
    void saveLegend(std::ofstream &output) const;

    repository::Experiment experiment;
    std::vector<repository::Report> reports;
    std::vector<repository::Exhibit> exhibits;
};
}
}

#endif