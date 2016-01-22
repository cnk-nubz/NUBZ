#ifndef SERVER_UTILS__REPORT_CHECKER__H
#define SERVER_UTILS__REPORT_CHECKER__H

#include <unordered_set>
#include <functional>

#include <boost/optional.hpp>

#include <utils/fp_algorithm.h>

#include <db/struct/Experiment.h>
#include <db/DatabaseSession.h>

#include <server/command/commons.h>

namespace server {
namespace utils {

class ReportChecker {
public:
    ReportChecker(db::DatabaseSession &session);
    SRV_CMD_CP_MV(ReportChecker);

    // returns if experiment with given id exists
    bool loadExperiment(std::int32_t experimentId);

    // returns if there is current experiment
    bool loadCurrentExperiment();

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkExhibitActionsIds(const Container &actionsIds) const;

    template <class Container,
              class = typename std::is_same<typename Container::value_type, std::int32_t>::type>
    bool checkBreakActionsIds(const Container &actionsIds) const;

private:
    db::DatabaseSession &session;
    boost::optional<db::Experiment> experiment;
};

template <class Container, class>
bool ReportChecker::checkExhibitActionsIds(const Container &actionsIds) const {
    using std::placeholders::_1;
    if (!experiment) {
        return false;
    }
    std::unordered_set<std::int32_t> correctIds;
    correctIds.insert(experiment.value().actions.begin(), experiment.value().actions.end());
    return ::utils::all_of(actionsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
    return false;
}

template <class Container, class>
bool ReportChecker::checkBreakActionsIds(const Container &actionsIds) const {
    using std::placeholders::_1;
    if (!experiment) {
        return false;
    }
    std::unordered_set<std::int32_t> correctIds;
    correctIds.insert(experiment.value().breakActions.begin(),
                      experiment.value().breakActions.end());
    return ::utils::all_of(actionsIds, std::bind(&decltype(correctIds)::count, &correctIds, _1));
}
}
}

#endif