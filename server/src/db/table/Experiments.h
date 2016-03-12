#ifndef DB_TABLE__EXPERIMENTS__H
#define DB_TABLE__EXPERIMENTS__H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>

#include "Column.h"
#include "Field.h"
#include "SqlCore.h"

namespace db {
namespace table {

struct Experiments {
    struct ContentData {
        struct Survey {
            std::vector<std::int32_t> typesOrder;
            std::vector<std::int32_t> simpleQuestions;
            std::vector<std::int32_t> sortQuestions;
            std::vector<std::int32_t> multipleChoiceQuestions;
        };

        ContentData() = default;
        ContentData(const std::string &jsonStr);
        operator std::string() const;

        std::vector<std::int32_t> actions;
        std::vector<std::int32_t> breakActions;
        Survey surveyBefore;
        Survey surveyAfter;
    };

    struct FieldID : detail::Field<std::int32_t, Experiments> {
        using detail::Field<std::int32_t, Experiments>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldID> ID{};

    struct FieldName : detail::Field<std::string, Experiments> {
        using detail::Field<std::string, Experiments>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldName> Name{};

    struct FieldState : detail::Field<std::int32_t, Experiments> {
        using detail::Field<std::int32_t, Experiments>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldState> State{};

    struct FieldStartDate : detail::OptField<boost::gregorian::date, Experiments> {
        using detail::OptField<boost::gregorian::date, Experiments>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldStartDate> StartDate{};

    struct FieldFinishDate : detail::OptField<boost::gregorian::date, Experiments> {
        using detail::OptField<boost::gregorian::date, Experiments>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldFinishDate> FinishDate{};

    struct FieldContent : detail::Field<ContentData, Experiments> {
        using detail::Field<ContentData, Experiments>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column2<FieldContent> Content{};

    static const std::string tableName;

    using Sql = detail::SqlCoreWithID<FieldID, FieldName, FieldState, FieldStartDate,
                                      FieldFinishDate, FieldContent>;
};
}
}

#endif