#include "Experiment.h"
#include "WrapperFactory.h"
#include "consts.h"

namespace utils {
namespace excel {

using namespace consts::header;

ExperimentInfoSection::ExperimentInfoSection() {
    columns.push_back(std::make_unique<ExperimentNameColumn>());
    columns.push_back(std::make_unique<StartDateColumn>());
    columns.push_back(std::make_unique<FinishDateColumn>());
}

wrapper_ptr ExperimentInfoSection::getHeader() const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &col : columns) {
        wrappers.push_back(col->getHeader());
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

wrapper_ptr ExperimentInfoSection::getEntry(const repository::Experiment &experiment) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &col : columns) {
        wrappers.push_back(col->getEntry(experiment));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

ExperimentNameColumn::ExperimentNameColumn() : ExperimentBased(experiment_info::experimentName) {
}

wrapper_ptr ExperimentNameColumn::getEntry(const repository::Experiment &experiment) const {
    return DataWrapperFactory::stringWrapper(experiment.name);
}

template <bool isStart>
DateColumn<isStart>::DateColumn()
    : ExperimentBased(isStart ? experiment_info::startDate : experiment_info::finishDate) {
}

template <bool isStart>
wrapper_ptr DateColumn<isStart>::getEntry(const repository::Experiment &experiment) const {
    if (auto date = isStart ? experiment.startDate : experiment.finishDate) {
        return DataWrapperFactory::dateWrapper(date.value());
    } else {
        return DataWrapperFactory::emptyWrapper();
    }
}

template struct DateColumn<true>;
template struct DateColumn<false>;
}
}