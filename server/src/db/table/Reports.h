#ifndef DB_TABLE__REPORTS__H
#define DB_TABLE__REPORTS__H

#include <cstdint>
#include <string>
#include <vector>

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
        struct Event {
            boost::optional<std::int32_t> exhibitID;
            std::int32_t beginHour;
            std::int32_t beginMin;
            std::int32_t beginSec;
            std::int32_t durationInSecs;
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

        ContentData() = default;
        ContentData(const std::string &jsonStr);
        operator std::string() const;

        std::vector<Event> history;
        SurveyAns surveyBefore;
        SurveyAns surveyAfter;
    };

    struct FieldID : detail::Field<std::int32_t, Reports> {
        using detail::Field<std::int32_t, Reports>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldID> ID{};

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

    using Sql = detail::SqlCore<FieldID, FieldExperimentID, FieldContent>;
};
}
}

#endif