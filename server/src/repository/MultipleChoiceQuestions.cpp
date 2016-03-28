#include <utils/fp_algorithm.h>

#include <db/table/MultipleChoiceQuestionOptions.h>
#include <db/table/MultipleChoiceQuestions.h>

#include "DefaultRepo.h"
#include "MultipleChoiceQuestions.h"
#include "error/DuplicateName.h"
#include "error/InvalidData.h"

namespace repository {

using MainTable = db::table::MultipleChoiceQuestions;
using OptionsTable = db::table::MultipleChoiceQuestionOptions;

using MainImpl = repository::detail::DefaultRepoWithID<MainTable>;
using OptionsImpl = repository::detail::DefaultRepoWithID<OptionsTable>;

namespace {
MainTable::Sql::in_t toDB(const MultipleChoiceQuestions::Question &question);
MultipleChoiceQuestions::Question fromDB(const MainTable::Sql::out_t &question);

OptionsTable::Sql::in_t optionToDB(const MultipleChoiceQuestions::Question::Option &option,
                                   std::int32_t questionID);
MultipleChoiceQuestions::Question::Option optionFromDB(const OptionsTable::Sql::out_t &option);
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
    auto getOptionsSql = OptionsTable::Sql::select().where(OptionsTable::QuestionID == questionID);

    auto result = std::vector<Question::Option>{};
    ::utils::transform(session.getResults(getOptionsSql), result, optionFromDB);
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
    checkOptions(question->options);
    checkName(question->name);

    question->ID = MainImpl::insert(session, toDB(*question));

    for (auto &option : question->options) {
        insert(&option, question->ID);
    }
}

void MultipleChoiceQuestions::checkName(const std::string &name) {
    if (name.empty()) {
        throw InvalidData{"question name cannot be empty"};
    }

    auto sql = db::sql::Select<MainTable::FieldName>{}.where(MainTable::Name == name);
    if (session.getResult(sql)) {
        throw DuplicateName{};
    }
}

void MultipleChoiceQuestions::checkOptions(const std::vector<Question::Option> &options) {
    if (options.size() < 2) {
        throw InvalidData{"question should have at least 2 options"};
    }

    std::unordered_set<std::string> optionsSet;
    for (auto &opt : options) {
        if (opt.text.empty()) {
            throw InvalidData{"question option cannot be empty"};
        }
        optionsSet.insert(opt.text);
    }
    if (options.size() != optionsSet.size()) {
        throw InvalidData{"question options should be unique"};
    }
}

void MultipleChoiceQuestions::insert(Question::Option *option, std::int32_t qID) {
    assert(option);
    option->ID = OptionsImpl::insert(session, optionToDB(*option, qID));
}

namespace {
MainTable::Sql::in_t toDB(const MultipleChoiceQuestions::Question &question) {
    return std::make_tuple(MainTable::FieldName{question.name},
                           MainTable::FieldQuestion{question.question},
                           MainTable::FieldSingleAnswer{question.singleAnswer});
}

MultipleChoiceQuestions::Question fromDB(const MainTable::Sql::out_t &question) {
    auto res = MultipleChoiceQuestions::Question{};
    res.ID = std::get<MainTable::FieldID>(question).value;
    res.name = std::get<MainTable::FieldName>(question).value;
    res.question = std::get<MainTable::FieldQuestion>(question).value;
    res.singleAnswer = std::get<MainTable::FieldSingleAnswer>(question).value;
    return res;
}

OptionsTable::Sql::in_t optionToDB(const MultipleChoiceQuestions::Question::Option &option,
                                   std::int32_t questionID) {
    return std::make_tuple(OptionsTable::FieldQuestionID{questionID},
                           OptionsTable::FieldText{option.text});
}

MultipleChoiceQuestions::Question::Option optionFromDB(const OptionsTable::Sql::out_t &option) {
    auto res = MultipleChoiceQuestions::Question::Option{};
    res.ID = std::get<OptionsTable::FieldID>(option).value;
    res.text = std::get<OptionsTable::FieldText>(option).value;
    return res;
}
}
}