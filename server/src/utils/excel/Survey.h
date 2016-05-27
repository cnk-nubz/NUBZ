#ifndef UTILS_EXCEL__SURVEY__H
#define UTILS_EXCEL__SURVEY__H

#include <repository/Experiments.h>

#include "column/QuestionBased.h"
#include "column/ReportBased.h"

namespace utils {
namespace excel {

template <bool isBefore>
class SimpleQuestion : public SimpleQuestionBased {
public:
    SimpleQuestion(const repository::SimpleQuestion &q);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(
        const repository::Report::SurveyAns::SimpleQAnswer &ans) const override;

private:
    repository::SimpleQuestion q;
};
extern template class SimpleQuestion<true>;
extern template class SimpleQuestion<false>;

template <bool isBefore>
class MultiChoiceQuestion : public MultipleChoiceQuestionBased {
public:
    MultiChoiceQuestion(const repository::MultipleChoiceQuestion &q);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(
        const repository::Report::SurveyAns::MultiChoiceQAnswer &ans) const override;

private:
    repository::MultipleChoiceQuestion q;
};
extern template class MultiChoiceQuestion<true>;
extern template class MultiChoiceQuestion<false>;

template <bool isBefore>
class SortQuestion : public SortQuestionBased {
public:
    SortQuestion(const repository::SortQuestion &q);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(
        const repository::Report::SurveyAns::SortQAnswer &ans) const override;

private:
    repository::SortQuestion q;
};
extern template class SortQuestion<true>;
extern template class SortQuestion<false>;

template <bool isBefore>
class Survey : public ReportBased {
    using SimpleQ = SimpleQuestion<isBefore>;
    using MultiChoiceQ = MultiChoiceQuestion<isBefore>;
    using SortQ = SortQuestion<isBefore>;

public:
    Survey(const repository::Experiment &experiment);
    virtual wrapper_ptr getHeader() const override;
    virtual wrapper_ptr getEntry(const repository::Report &report) const override;

private:
    repository::Experiment::Survey survey;
    std::vector<std::unique_ptr<SimpleQuestionBased>> simpleQCols;
    std::vector<std::unique_ptr<MultipleChoiceQuestionBased>> multiChoiceQCols;
    std::vector<std::unique_ptr<SortQuestionBased>> sortQCols;
};
extern template class Survey<true>;
extern template class Survey<false>;
using SurveyBefore = Survey<true>;
using SurveyAfter = Survey<false>;
}
}

#endif