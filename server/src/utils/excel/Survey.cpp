#include "Survey.h"
#include "WrapperFactory.h"

namespace utils {
namespace excel {

template <bool isBefore>
Survey<isBefore>::Survey(const repository::Experiment &experiment)
    : survey(isBefore ? experiment.surveyBefore : experiment.surveyAfter) {
    for (const auto &q : survey.simpleQuestions) {
        simpleQCols.push_back(std::make_unique<SimpleQ>(q));
    }
    for (const auto &q : survey.multipleChoiceQuestions) {
        multiChoiceQCols.push_back(std::make_unique<MultiChoiceQ>(q));
    }
    for (const auto &q : survey.sortQuestions) {
        sortQCols.push_back(std::make_unique<SortQ>(q));
    }
}

template <bool _>
wrapper_ptr Survey<_>::getHeader() const {
    auto wrappers = std::vector<wrapper_ptr>{};

    auto simpleIt = simpleQCols.begin();
    auto multiIt = multiChoiceQCols.begin();
    auto sortIt = sortQCols.begin();
    for (auto &type : survey.typesOrder) {
        switch (type) {
            case repository::Experiment::Survey::QuestionType::Simple:
                wrappers.push_back((*simpleIt++)->getHeader());
                break;
            case repository::Experiment::Survey::QuestionType::MultipleChoice:
                wrappers.push_back((*multiIt++)->getHeader());
                break;
            case repository::Experiment::Survey::QuestionType::Sort:
                wrappers.push_back((*sortIt++)->getHeader());
                break;
        }
    }

    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template <bool isBefore>
wrapper_ptr Survey<isBefore>::getEntry(const repository::Report &report) const {
    auto &ans = isBefore ? report.surveyBefore : report.surveyAfter;
    auto wrappers = std::vector<wrapper_ptr>{};

    auto simpleQIt = ans.simpleQAnswers.begin();
    auto multiQIt = ans.multiChoiceQAnswers.begin();
    auto sortQIt = ans.sortQAnswers.begin();

    auto simpleIt = simpleQCols.begin();
    auto multiIt = multiChoiceQCols.begin();
    auto sortIt = sortQCols.begin();
    for (auto &type : survey.typesOrder) {
        switch (type) {
            case repository::Experiment::Survey::QuestionType::Simple:
                wrappers.push_back((*simpleIt++)->getEntry(*simpleQIt++));
                break;
            case repository::Experiment::Survey::QuestionType::MultipleChoice:
                wrappers.push_back((*multiIt++)->getEntry(*multiQIt++));
                break;
            case repository::Experiment::Survey::QuestionType::Sort:
                wrappers.push_back((*sortIt++)->getEntry(*sortQIt++));
                break;
        }
    }

    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template class Survey<true>;
template class Survey<false>;

template <bool isBefore>
SimpleQuestion<isBefore>::SimpleQuestion(const repository::SimpleQuestion &q) : q(q) {
}

template <bool isBefore>
wrapper_ptr SimpleQuestion<isBefore>::getHeader() const {
    auto period =
        isBefore ? HeaderWrapperFactory::Period::Before : HeaderWrapperFactory::Period::After;
    return HeaderWrapperFactory::simpleQuestionWrapper(period, q.name);
}

template <bool _>
wrapper_ptr SimpleQuestion<_>::getEntry(
    const repository::Report::SurveyAns::SimpleQAnswer &ans) const {
    return DataWrapperFactory::stringWrapper(ans);
}

template class SimpleQuestion<true>;
template class SimpleQuestion<false>;

template <bool _>
MultiChoiceQuestion<_>::MultiChoiceQuestion(const repository::MultipleChoiceQuestion &q) : q(q) {
}

template <bool isBefore>
wrapper_ptr MultiChoiceQuestion<isBefore>::getHeader() const {
    auto period =
        isBefore ? HeaderWrapperFactory::Period::Before : HeaderWrapperFactory::Period::After;
    auto type = HeaderWrapperFactory::QType::Multi;

    auto wrappers = std::vector<wrapper_ptr>{};
    for (std::size_t i = 0; i < q.options.size(); i++) {
        wrappers.push_back(HeaderWrapperFactory::questionWrapper(period, type, q.name, i + 1));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template <bool _>
wrapper_ptr MultiChoiceQuestion<_>::getEntry(
    const repository::Report::SurveyAns::MultiChoiceQAnswer &ans) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &option : q.options) {
        bool choosen = utils::count(ans, option.ID) == 1;
        wrappers.push_back(DataWrapperFactory::questionOptionWrapper(choosen));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template class MultiChoiceQuestion<true>;
template class MultiChoiceQuestion<false>;

template <bool _>
SortQuestion<_>::SortQuestion(const repository::SortQuestion &q) : q(q) {
}

template <bool isBefore>
wrapper_ptr SortQuestion<isBefore>::getHeader() const {
    auto period =
        isBefore ? HeaderWrapperFactory::Period::Before : HeaderWrapperFactory::Period::After;
    auto type = HeaderWrapperFactory::QType::Sort;

    auto wrappers = std::vector<wrapper_ptr>{};
    for (std::size_t i = 0; i < q.options.size(); i++) {
        wrappers.push_back(HeaderWrapperFactory::questionWrapper(period, type, q.name, i + 1));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template <bool _>
wrapper_ptr SortQuestion<_>::getEntry(const repository::Report::SurveyAns::SortQAnswer &ans) const {
    auto wrappers = std::vector<wrapper_ptr>{};
    for (auto &option : q.options) {
        std::int32_t pos = (std::int32_t)utils::index(ans, option.ID) + 1;
        wrappers.push_back(DataWrapperFactory::numWrapper(pos));
    }
    return ContainerWrapperFactory::rowWrapper(std::move(wrappers));
}

template class SortQuestion<true>;
template class SortQuestion<false>;
}
}