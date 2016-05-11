#include "Report.h"
#include "Survey.h"
#include "WrapperFactory.h"
#include "consts.h"

namespace utils {
namespace excel {

using namespace consts::header;

ReportInfoSection::ReportInfoSection(const repository::Experiment &experiment) {
    columns.push_back(std::make_unique<ReportIDColumn>());
    columns.push_back(std::make_unique<ReportReceiveDateColumn>());
    // columns.push_back(std::make_unique<ReportBeginTimeColumn>());
    columns.push_back(std::make_unique<ReportFinishTimeColumn>());
    columns.push_back(std::make_unique<SurveyBefore>(experiment));
    columns.push_back(std::make_unique<SurveyAfter>(experiment));
}

wrapper_ptr ReportInfoSection::getHeader() const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (const auto &col : columns) {
        wrappers.push_back(col->getHeader());
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

wrapper_ptr ReportInfoSection::getEntry(const repository::Report &report) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (const auto &col : columns) {
        wrappers.push_back(col->getEntry(report));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

ReportIDColumn::ReportIDColumn() : ReportBased(report_info::reportID) {
}

wrapper_ptr ReportIDColumn::getEntry(const repository::Report &report) const {
    return DataWrapperFactory::numWrapper(report.ID);
}

ReportReceiveDateColumn::ReportReceiveDateColumn() : ReportBased(report_info::receiveDate) {
}

wrapper_ptr ReportReceiveDateColumn::getEntry(const repository::Report &report) const {
    return DataWrapperFactory::dateWrapper(report.receiveDate);
}

template <bool isBegin>
ReportTimeColumn<isBegin>::ReportTimeColumn()
    : ReportBased(isBegin ? report_info::beginTime : report_info::finishTime) {
}

template <bool isBegin>
wrapper_ptr ReportTimeColumn<isBegin>::getEntry(const repository::Report &report) const {
    return DataWrapperFactory::timeWrapper(isBegin ? report.beginTime : report.finishTime);
}

template struct ReportTimeColumn<true>;
template struct ReportTimeColumn<false>;
}
}