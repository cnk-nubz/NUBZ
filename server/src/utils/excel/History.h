#ifndef UTILS_EXCEL__HISTORY__H
#define UTILS_EXCEL__HISTORY__H

#include <cstdint>
#include <string>
#include <unordered_map>

#include <repository/Exhibits.h>
#include <repository/Experiments.h>

#include "column/EventBased.h"

namespace utils {
namespace excel {

/*
 * HistorySection:
 *
 * exhibit name
 * begin
 * end
 * duration
 * actions
 * break actions
 * is break
 */
class HistorySection : public EventBased {
public:
    HistorySection(const repository::Experiment &experiment,
                   const std::vector<repository::Exhibit> &exhibits);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;

private:
    std::vector<std::unique_ptr<EventBased>> columns;
};

class ExhibitNameColumn : public EventBased {
public:
    ExhibitNameColumn(const std::vector<repository::Exhibit> &exhibits);
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;

private:
    std::unordered_map<std::int32_t, std::string> exhibitsNames;
};

template <bool isBreak>
class ActionsSection : public EventBased {
public:
    ActionsSection(const std::vector<repository::Action> &actions);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;

private:
    const std::vector<repository::Action> actions;
};
extern template class ActionsSection<true>;
extern template class ActionsSection<false>;
using ExhibitActionsColumn = ActionsSection<true>;
using BreakActionsColumn = ActionsSection<false>;

template <bool isBegin>
struct TimeColumn : EventBased {
    TimeColumn();
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;
};
extern template struct TimeColumn<true>;
extern template struct TimeColumn<false>;
using BeginTimeColumn = TimeColumn<true>;
using EndTimeColumn = TimeColumn<false>;

struct DurationColumn : EventBased {
    DurationColumn();
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;
};

struct EventTypeColumn : EventBased {
    EventTypeColumn();
    virtual wrapper_ptr getEntry(const repository::Report::Event &event) const override;
};
}
}

#endif