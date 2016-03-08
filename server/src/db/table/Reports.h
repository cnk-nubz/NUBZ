#ifndef DB_TABLE__REPORTS__H
#define DB_TABLE__REPORTS__H

#include <tuple>
#include <vector>

#include <db/sql/sql.h>

#include <boost/optional.hpp>

#include "Column.h"
#include "Value.h"

#include "Experiments.h"

/*
 * ColumnExperimentId references Experiments::ColumnId (on delete cascade)
 */

namespace db {
namespace table {

struct Reports {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueExperimentId : detail::Value<Experiments::ValueId::type> {
        using detail::Value<Experiments::ValueId::type>::Value;
    };

    struct ValueContent : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, Reports, ValueId> {
        static const std::string name;
    };

    struct ColumnExperimentId : detail::Column<ColumnExperimentId, Reports, ValueExperimentId> {
        static const std::string name;
    };

    struct ColumnContent : detail::Column<ColumnContent, Reports, ValueContent> {
        static const std::string name;
    };

    struct Row {
        struct Event {
            boost::optional<std::int32_t> exhibitID;
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

        ValueId::type ID;
        ValueExperimentId::type experimentID;
        std::vector<Event> history;
        SurveyAns surveyBefore;
        SurveyAns surveyAfter;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueExperimentId, ValueContent>;
        using DBIn = std::tuple<ValueId, ValueExperimentId, ValueContent>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnExperimentId colExperimentId;
    static const ColumnContent colContent;

    static auto select() {
        return ::db::sql::Select<ColumnId, ColumnExperimentId, ColumnContent>();
    }

    static auto insert() {
        return ::db::sql::Insert<ColumnId, ColumnExperimentId, ColumnContent>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId, ColumnExperimentId, ColumnContent>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId, ColumnExperimentId, ColumnContent>();
    }
};
}
}

#endif