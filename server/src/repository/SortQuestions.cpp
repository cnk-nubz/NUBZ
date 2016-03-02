#include <utils/fp_algorithm.h>

#include <db/table/SortQuestionOptions.h>
#include <db/table/SortQuestions.h>

#include "DefaultRepo.h"
#include "SortQuestions.h"

namespace repository {

using MainTable = db::table::SortQuestions;
using OptionsTable = db::table::SortQuestionOptions;

using MainImpl = repository::detail::DefaultRepoWithID<MainTable>;
using OptionsImpl = repository::detail::DefaultRepoWithID<OptionsTable>;

namespace {
MainTable::Row toDB(const SortQuestions::Question &question);
SortQuestions::Question fromDB(const MainTable::Row &question);

OptionsTable::Row optionToDB(const SortQuestions::Question::Option &option,
                             std::int32_t questionID);
SortQuestions::Question::Option optionFromDB(const OptionsTable::Row &option);
}

SortQuestions::SortQuestions(db::DatabaseSession &session) : session(session) {
}

boost::optional<SortQuestions::Question> SortQuestions::get(std::int32_t ID) {
    if (auto res = MainImpl::get(session, ID)) {
        auto question = fromDB(res.value());
        question.options = getOptions(question.ID);
        return question;
    } else {
        return {};
    }
}

std::vector<SortQuestions::Question> SortQuestions::getAll() {
    std::vector<SortQuestions::Question> result;
    utils::transform(MainImpl::getAll(session), result, fromDB);
    for (auto &q : result) {
        q.options = getOptions(q.ID);
    }
    return result;
}

std::vector<SortQuestions::Question::Option> SortQuestions::getOptions(std::int32_t questionID) {
    std::vector<OptionsTable::Row> dbOptions;
    auto getOptionsSql = OptionsTable::select().where(OptionsTable::colQuestionId == questionID);
    ::utils::transform(
        session.getResults(getOptionsSql), dbOptions, OptionsTable::RowFactory::fromDB);

    std::vector<Question::Option> result;
    ::utils::transform(dbOptions, result, optionFromDB);
    return result;
}

void SortQuestions::remove(std::int32_t ID) {
    MainImpl::remove(session, ID);
}

void SortQuestions::removeAll() {
    MainImpl::removeAll(session);
}

void SortQuestions::insert(std::vector<SortQuestions::Question> *questions) {
    assert(questions);
    for (auto &question : *questions) {
        insert(&question);
    }
}

void SortQuestions::insert(SortQuestions::Question *question) {
    assert(question);
    question->ID = MainImpl::insert(session, toDB(*question));
    for (auto &option : question->options) {
        insert(&option, question->ID);
    }
}

void SortQuestions::insert(Question::Option *option, std::int32_t qID) {
    assert(option);
    option->ID = OptionsImpl::insert(session, optionToDB(*option, qID));
}

namespace {
MainTable::Row toDB(const SortQuestions::Question &question) {
    MainTable::Row res;
    res.name = question.name;
    res.question = question.question;
    return res;
}

SortQuestions::Question fromDB(const MainTable::Row &question) {
    SortQuestions::Question res;
    res.ID = question.ID;
    res.name = question.name;
    res.question = question.question;
    return res;
}

OptionsTable::Row optionToDB(const SortQuestions::Question::Option &option,
                             std::int32_t questionID) {
    OptionsTable::Row res;
    res.questionID = questionID;
    res.text = option.text;
    return res;
}

SortQuestions::Question::Option optionFromDB(const OptionsTable::Row &option) {
    SortQuestions::Question::Option res;
    res.ID = option.ID;
    res.text = option.text;
    return res;
}
}
}