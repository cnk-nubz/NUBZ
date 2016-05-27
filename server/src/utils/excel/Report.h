#ifndef UTILS_EXCEL__REPORT__H
#define UTILS_EXCEL__REPORT__H

#include <repository/Experiments.h>

#include "column/ReportBased.h"

namespace utils {
namespace excel {

/*
 * ReportInfoSection:
 *
 * report id
 * report receive date
 * report finish time
 * survey before
 * survey after
 */
class ReportInfoSection : public ReportBased {
public:
    ReportInfoSection(const repository::Experiment &experiment);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(const repository::Report &report) const override;

private:
    std::vector<std::unique_ptr<ReportBased>> columns;
};

struct ReportIDColumn : ReportBased {
    ReportIDColumn();
    virtual wrapper_ptr getEntry(const repository::Report &report) const override;
};

struct ReportReceiveDateColumn : ReportBased {
    ReportReceiveDateColumn();
    virtual wrapper_ptr getEntry(const repository::Report &report) const override;
};

template <bool isBegin>
struct ReportTimeColumn : ReportBased {
    ReportTimeColumn();
    virtual wrapper_ptr getEntry(const repository::Report &report) const override;
};
extern template struct ReportTimeColumn<true>;
extern template struct ReportTimeColumn<false>;
using ReportBeginTimeColumn = ReportTimeColumn<true>;
using ReportFinishTimeColumn = ReportTimeColumn<false>;
}
}

#endif