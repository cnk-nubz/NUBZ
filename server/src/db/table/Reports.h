#ifndef DB_TABLE__REPORTS__H
#define DB_TABLE__REPORTS__H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

/*
 * ExperimentID references Experiments::ID (on delete cascade)
 */

namespace db {
namespace table {

struct Reports {
    struct ContentData {
        struct Time {
            std::int32_t h, m, s;
        };

        struct Event {
            boost::optional<std::int32_t> exhibitID;
            Time beginTime;
            std::int32_t durationInSecs;
            std::vector<std::int32_t> actions;
        };

        struct SurveyAns {
            using SimpleQAnswer = std::string;
            using MultiChoiceQAnswer = std::vector<std::int32_t>;
            using SortQAnswer = std::vector<std::int32_t>;

            std::vector<SimpleQAnswer> simpleQAnswers;
            std::vector<MultiChoiceQAnswer> multiChoiceQAnswers;
            std::vector<SortQAnswer> sortQAnswers;
        };

        ContentData() = default;
        ContentData(const std::string &jsonStr);
        operator std::string() const;

        Time beginTime;
        Time finishTime;

        std::vector<Event> history;
        SurveyAns surveyBefore;
        SurveyAns surveyAfter;
    };

    struct FieldID : detail::Field<std::int32_t, Reports> {
        using detail::Field<std::int32_t, Reports>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldID> ID{};

    struct FieldReceiveDate : detail::Field<boost::gregorian::date, Reports> {
        using detail::Field<boost::gregorian::date, Reports>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldReceiveDate> ReceiveDate{};

    struct FieldExperimentID : detail::Field<std::int32_t, Reports> {
        using detail::Field<std::int32_t, Reports>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldExperimentID> ExperimentID{};

    struct FieldContent : detail::Field<ContentData, Reports> {
        using detail::Field<ContentData, Reports>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldContent> Content{};

    static const std::string tableName;

    using Sql = detail::SqlCore<FieldID, FieldReceiveDate, FieldExperimentID, FieldContent>;
};
}
}

#endif