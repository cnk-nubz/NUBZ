#include <utils/fp_algorithm.h>

#include <db/table/MultipleChoiceQuestionOptions.h>
#include <db/table/MultipleChoiceQuestions.h>

#include "DefaultRepo.h"
#include "MultipleChoiceQuestions.h"

namespace repository {

using MainTable = db::table::MultipleChoiceQuestions;
using OptionsTable = db::table::MultipleChoiceQuestionOptions;

using MainImpl = repository::detail::DefaultRepoWithID<MainTable>;
using OptionsImpl = repository::detail::DefaultRepoWithID<OptionsTable>;

namespace {
MainTable::Row toDB(const MultipleChoiceQuestions::Question &question);
MultipleChoiceQuestions::Question fromDB(const MainTable::Row &question);

OptionsTable::Row optionToDB(const MultipleChoiceQuestions::Question::Option &option,
                             std::int32_t questionID);
MultipleChoiceQuestions::Question::Option optionFromDB(const OptionsTable::Row &option);
}

MultipleChoiceQuestions::MultipleChoiceQuestions(db::DatabaseSession &session) : session(session) {
}

boost::optional<MultipleChoiceQuestions::Question> MultipleChoiceQuestions::get(std::int32_t ID) {
    if (auto res = MainImpl::get(session, ID)) {
        auto question = fromDB(res.value());
        question.options = getOptions(question.ID);
        return question;
    } else {
        return {};
    }
}

std::vector<MultipleChoiceQuestions::Question> MultipleChoiceQuestions::getAll() {
    auto result = std::vector<MultipleChoiceQuestions::Question>{};
    utils::transform(MainImpl::getAll(session), result, fromDB);
    for (auto &q : result) {
        q.options = getOptions(q.ID);
    }
    return result;
}

std::vector<MultipleChoiceQuestions::Question::Option> MultipleChoiceQuestions::getOptions(
    std::int32_t questionID) {
    auto dbOptions = std::vector<OptionsTable::Row>{};
    auto getOptionsSql = OptionsTable::select().where(OptionsTable::colQuestionId == questionID);
    ::utils::transform(
        session.getResults(getOptionsSql), dbOptions, OptionsTable::RowFactory::fromDB);

    auto result = std::vector<Question::Option>{};
    ::utils::transform(dbOptions, result, optionFromDB);
    return result;
}

void MultipleChoiceQuestions::remove(std::int32_t ID) {
    MainImpl::remove(session, ID);
}

void MultipleChoiceQuestions::removeAll() {
    MainImpl::removeAll(session);
}

void MultipleChoiceQuestions::insert(std::vector<MultipleChoiceQuestions::Question> *questions) {
    assert(questions);
    for (auto &question : *questions) {
        insert(&question);
    }
}

void MultipleChoiceQuestions::insert(MultipleChoiceQuestions::Question *question) {
    assert(question);
    question->ID = MainImpl::insert(session, toDB(*question));
    for (auto &option : question->options) {
        insert(&option, question->ID);
    }
}

void MultipleChoiceQuestions::insert(Question::Option *option, std::int32_t qID) {
    assert(option);
    option->ID = OptionsImpl::insert(session, optionToDB(*option, qID));
}

namespace {
MainTable::Row toDB(const MultipleChoiceQuestions::Question &question) {
    auto res = MainTable::Row{};
    res.name = question.name;
    res.question = question.question;
    res.singleAnswer = question.singleAnswer;
    return res;
}

MultipleChoiceQuestions::Question fromDB(const MainTable::Row &question) {
    auto res = MultipleChoiceQuestions::Question{};
    res.ID = question.ID;
    res.name = question.name;
    res.question = question.question;
    res.singleAnswer = question.singleAnswer;
    return res;
}

OptionsTable::Row optionToDB(const MultipleChoiceQuestions::Question::Option &option,
                             std::int32_t questionID) {
    auto res = OptionsTable::Row();
    res.questionID = questionID;
    res.text = option.text;
    return res;
}

MultipleChoiceQuestions::Question::Option optionFromDB(const OptionsTable::Row &option) {
    auto res = MultipleChoiceQuestions::Question::Option{};
    res.ID = option.ID;
    res.text = option.text;
    return res;
}
}
}