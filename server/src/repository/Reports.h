#ifndef REPOSITORY__REPORTS__H
#define REPOSITORY__REPORTS__H

#include <cstdint>
#include <stdexcept>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

#include "Experiments.h"

namespace repository {

class Reports {
public:
    struct Report {
        struct Event {
            struct TimePoint {
                std::int32_t h;
                std::int32_t m;
                std::int32_t s;
            };

            // foreign key
            boost::optional<std::int32_t> exhibitID;

            TimePoint beginTime;
            std::int32_t durationInSecs;

            // foreign keys
            std::vector<std::int32_t> actions;
        };

        struct SurveyAns {
            using SimpleQAnswer = boost::optional<std::string>;
            using MultiChoiceQAnswer = boost::optional<std::vector<std::int32_t>>;
            using SortQAnswer = boost::optional<std::vector<std::int32_t>>;

            std::vector<SimpleQAnswer> simpleQAnswers;
            std::vector<MultiChoiceQAnswer> multiChoiceQAnswers;
            std::vector<SortQAnswer> sortQAnswers;
        };

        std::int32_t ID;

        // foreign key
        std::int32_t experimentID;

        std::vector<Event> history;
        SurveyAns surveyBefore;
        SurveyAns surveyAfter;
    };

    Reports(db::DatabaseSession &session);

    bool exist(std::int32_t ID);

    boost::optional<Report> get(std::int32_t ID);
    std::vector<Report> getAllForExperiment(std::int32_t experimentID);

    // may throw InvalidData in case of incorrect data
    void insert(const Report &report);

private:
    // throws InvalidData in case of greater id than reports id counter
    void checkID(std::int32_t ID);

    void checkExhibits(const std::vector<Report::Event> &events);

    void checkEvents(const Experiment &experiment, const std::vector<Report::Event> &events);

    // throws InvalidData in case of invalid id
    void checkActions(const Experiment &experiment, const std::vector<Report::Event> &events);

    // throws InvalidData in case of invalid id
    void checkBreakActions(const Experiment &experiment, const std::vector<Report::Event> &events);

    // throws InvalidData in case of value less than 0
    void checkDurations(const std::vector<Report::Event> &events) const;

    void checkTimePoint(const Report::Event::TimePoint &timePoint) const;

    void checkSurveyAns(const Experiment::Survey &survey, const Report::SurveyAns &surveyAns);

    static void checkSimpleAns(const SimpleQuestion &question,
                               const Report::SurveyAns::SimpleQAnswer &answer);
    static void checkMultipleChoiceAns(const MultipleChoiceQuestion &question,
                                       const Report::SurveyAns::MultiChoiceQAnswer &answer);
    static void checkSortAns(const SortQuestion &question,
                             const Report::SurveyAns::SortQAnswer &answer);

    db::DatabaseSession &session;
};

using Report = Reports::Report;
}

#endif