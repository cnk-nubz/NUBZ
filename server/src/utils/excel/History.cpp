#include <utils/fp_algorithm.h>

#include "History.h"
#include "WrapperFactory.h"
#include "column/Column.h"
#include "consts.h"

namespace utils {
namespace excel {

using namespace consts::header;

ExhibitNameColumn::ExhibitNameColumn(const std::vector<repository::Exhibit> &exhibits)
    : EventBased(event::exhibitName) {
    for (auto &exhibit : exhibits) {
        exhibitsNames[exhibit.ID] = exhibit.name;
    }
}

wrapper_ptr ExhibitNameColumn::getEntry(const repository::Report::Event &event) const {
    if (event.exhibitID) {
        auto name = exhibitsNames.at(event.exhibitID.value());
        return DataWrapperFactory::stringWrapper(name);
    } else {
        return DataWrapperFactory::emptyWrapper();
    }
}

template <bool isBreak>
ActionsSection<isBreak>::ActionsSection(const std::vector<repository::Action> &actions)
    : actions(actions) {
}

template <bool isBreak>
wrapper_ptr ActionsSection<isBreak>::getHeader() const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &action : actions) {
        if (isBreak) {
            wrappers.push_back(HeaderWrapperFactory::breakActionWrapper(action.text));
        } else {
            wrappers.push_back(HeaderWrapperFactory::exhibitActionWrapper(action.text));
        }
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template <bool isBreak>
wrapper_ptr ActionsSection<isBreak>::getEntry(const repository::Report::Event &event) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto action : actions) {
        bool occured = utils::count(event.actions, action.ID) == 1;
        wrappers.push_back(DataWrapperFactory::actionWrapper(occured));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template class ActionsSection<true>;
template class ActionsSection<false>;

template <bool isBegin>
TimeColumn<isBegin>::TimeColumn() : EventBased(isBegin ? event::beginTime : event::endTime) {
}

template <bool isBegin>
wrapper_ptr TimeColumn<isBegin>::getEntry(const repository::Report::Event &event) const {
    return DataWrapperFactory::timeWrapper(isBegin ? event.beginTime
                                                   : event.beginTime.after(event.durationInSecs));
}

template struct TimeColumn<true>;
template struct TimeColumn<false>;

DurationColumn::DurationColumn() : EventBased(event::duration) {
}

wrapper_ptr DurationColumn::getEntry(const repository::Report::Event &event) const {
    return DataWrapperFactory::numWrapper(event.durationInSecs);
}

EventTypeColumn::EventTypeColumn() : EventBased(event::isBreak) {
}

wrapper_ptr EventTypeColumn::getEntry(const repository::Report::Event &event) const {
    return DataWrapperFactory::eventType(!event.exhibitID);
}

HistorySection::HistorySection(const repository::Experiment &experiment,
                               const std::vector<repository::Exhibit> &exhibits) {
    columns.push_back(std::make_unique<ExhibitNameColumn>(exhibits));
    columns.push_back(std::make_unique<BeginTimeColumn>());
    columns.push_back(std::make_unique<EndTimeColumn>());
    columns.push_back(std::make_unique<DurationColumn>());
    columns.push_back(std::make_unique<ExhibitActionsColumn>(experiment.actions));
    columns.push_back(std::make_unique<BreakActionsColumn>(experiment.breakActions));
    columns.push_back(std::make_unique<EventTypeColumn>());
}

wrapper_ptr HistorySection::getHeader() const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &col : columns) {
        wrappers.push_back(col->getHeader());
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

wrapper_ptr HistorySection::getEntry(const repository::Report::Event &event) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &col : columns) {
        wrappers.push_back(col->getEntry(event));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}
}
}