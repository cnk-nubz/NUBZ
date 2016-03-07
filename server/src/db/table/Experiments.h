#ifndef DB_TABLE__EXPERIMENTS__H
#define DB_TABLE__EXPERIMENTS__H

#include <tuple>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>

#include <db/sql/sql.h>

#include "Column.h"
#include "Value.h"

namespace db {
namespace table {

struct Experiments {
    struct ValueId : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueName : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ValueState : detail::Value<std::int32_t> {
        using detail::Value<std::int32_t>::Value;
    };

    struct ValueStartDate : detail::OptValue<boost::gregorian::date> {
        using detail::OptValue<boost::gregorian::date>::OptValue;
    };

    struct ValueFinishDate : detail::OptValue<boost::gregorian::date> {
        using detail::OptValue<boost::gregorian::date>::OptValue;
    };

    struct ValueContent : detail::Value<std::string> {
        using detail::Value<std::string>::Value;
    };

    struct ColumnId : detail::Column<ColumnId, Experiments, ValueId> {
        static const std::string name;
    };

    struct ColumnName : detail::Column<ColumnName, Experiments, ValueName> {
        static const std::string name;
    };

    struct ColumnState : detail::Column<ColumnState, Experiments, ValueState> {
        static const std::string name;
    };

    struct ColumnStartDate : detail::Column<ColumnStartDate, Experiments, ValueStartDate> {
        static const std::string name;
    };

    struct ColumnFinishDate : detail::Column<ColumnFinishDate, Experiments, ValueFinishDate> {
        static const std::string name;
    };

    struct ColumnContent : detail::Column<ColumnContent, Experiments, ValueContent> {
        static const std::string name;
    };

    struct Row {
        struct Survey {
            std::vector<std::int32_t> typesOrder;
            std::vector<std::int32_t> simpleQuestions;
            std::vector<std::int32_t> sortQuestions;
            std::vector<std::int32_t> multipleChoiceQuestions;
        };

        ValueId::type ID;
        ValueName::type name;
        ValueState::type state;
        ValueStartDate::type startDate;
        ValueFinishDate::type finishDate;

        std::vector<std::int32_t> actions;
        std::vector<std::int32_t> breakActions;
        Survey surveyBefore;
        Survey surveyAfter;
    };

    struct RowFactory {
        using DBOut = std::tuple<ValueId, ValueName, ValueState, ValueStartDate, ValueFinishDate,
                                 ValueContent>;
        using DBIn =
            std::tuple<ValueName, ValueState, ValueStartDate, ValueFinishDate, ValueContent>;
        static Row fromDB(const DBOut &dbOut);
        static DBIn toDB(const Row &row);
    };

    static const std::string tableName;
    static const ColumnId colId;
    static const ColumnName colName;
    static const ColumnState colState;
    static const ColumnStartDate colStartDate;
    static const ColumnFinishDate colFinishDate;
    static const ColumnContent colContent;

    static auto select() {
        return ::db::sql::Select<ColumnId,
                                 ColumnName,
                                 ColumnState,
                                 ColumnStartDate,
                                 ColumnFinishDate,
                                 ColumnContent>();
    }

    static auto insert() {
        return ::db::sql::
            Insert<ColumnName, ColumnState, ColumnStartDate, ColumnFinishDate, ColumnContent>();
    }

    static auto del() {
        return ::db::sql::Delete<ColumnId,
                                 ColumnName,
                                 ColumnState,
                                 ColumnStartDate,
                                 ColumnFinishDate,
                                 ColumnContent>();
    }

    static auto update() {
        return ::db::sql::Update<ColumnId,
                                 ColumnName,
                                 ColumnState,
                                 ColumnStartDate,
                                 ColumnFinishDate,
                                 ColumnContent>();
    }
};
}
}

#endif