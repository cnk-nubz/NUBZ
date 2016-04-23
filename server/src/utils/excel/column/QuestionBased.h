#ifndef UTILS_EXCEL_COLUMN__QUESTION_BASED__H
#define UTILS_EXCEL_COLUMN__QUESTION_BASED__H

#include <repository/Reports.h>

#include "Column.h"

namespace utils {
namespace excel {

class SimpleQuestionBased : public Column {
public:
    virtual wrapper_ptr getEntry(const repository::Report::SurveyAns::SimpleQAnswer &ans) const = 0;

protected:
    using Column::Column;
};

class MultipleChoiceQuestionBased : public Column {
public:
    virtual wrapper_ptr getEntry(
        const repository::Report::SurveyAns::MultiChoiceQAnswer &ans) const = 0;

protected:
    using Column::Column;
};

class SortQuestionBased : public Column {
public:
    virtual wrapper_ptr getEntry(const repository::Report::SurveyAns::SortQAnswer &ans) const = 0;

protected:
    using Column::Column;
};
}
}

#endif