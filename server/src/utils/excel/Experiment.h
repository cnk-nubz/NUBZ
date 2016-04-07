#ifndef UTILS_EXCEL__EXPERIMENT__H
#define UTILS_EXCEL__EXPERIMENT__H

#include <repository/Experiments.h>

#include "column/ExperimentBased.h"

namespace utils {
namespace excel {

/*
 * ExperimentInfoSection:
 *
 * experiment name
 * start date
 * finish date
 */
class ExperimentInfoSection : public ExperimentBased {
public:
    ExperimentInfoSection();
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(const repository::Experiment &experiment) const override;

private:
    std::vector<std::unique_ptr<ExperimentBased>> columns;
};

struct ExperimentNameColumn : ExperimentBased {
    ExperimentNameColumn();
    virtual wrapper_ptr getEntry(const repository::Experiment &experiment) const override;
};

template <bool isStart>
struct DateColumn : ExperimentBased {
    DateColumn();
    virtual wrapper_ptr getEntry(const repository::Experiment &experiment) const override;
};
extern template struct DateColumn<true>;
extern template struct DateColumn<false>;
using StartDateColumn = DateColumn<true>;
using FinishDateColumn = DateColumn<false>;
}
}

#endif