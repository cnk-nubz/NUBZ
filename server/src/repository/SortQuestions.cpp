#include <utils/fp_algorithm.h>

#include <db/table/SortQuestionOptions.h>
#include <db/table/SortQuestions.h>

#include "DefaultRepo.h"
#include "SortQuestions.h"
#include "error/DuplicateName.h"
#include "error/InUse.h"
#include "error/InvalidData.h"

namespace repository {

using MainTable = db::table::SortQuestions;
using OptionsTable = db::table::SortQuestionOptions;

using MainImpl = repository::detail::DefaultRepoIDRefCount<MainTable>;
using OptionsImpl = repository::detail::DefaultRepoID<OptionsTable>;

namespace {
MainTable::Sql::in_t toDB(const SortQuestions::Question &question);
SortQuestions::Question fromDB(const MainTable::Sql::out_t &question);

OptionsTable::Sql::in_t optionToDB(const SortQuestions::Question::Option &option,
                                   std::int32_t questionID);
SortQuestions::Question::Option optionFromDB(const OptionsTable::Sql::out_t &option);
}

SortQuestions::SortQuestions(db::DatabaseSession &session) : session(session) {
}

void SortQuestions::incReferenceCount(std::int32_t ID) {
    MainImpl::incRefCount(session, ID);
}

void SortQuestions::decReferenceCount(std::int32_t ID) {
    MainImpl::decRefCount(session, ID);
}

SortQuestions::Question SortQuestions::get(std::int32_t ID) {
    if (auto res = getOpt(ID)) {
        return res.value();
    } else {
        throw InvalidData{"incorrect question ID"};
    }
}

boost::optional<SortQuestions::Question> SortQuestions::getOpt(std::int32_t ID) {
    if (auto res = MainImpl::get(session, ID)) {
        auto question = fromDB(res.value());
        question.options = getOptions(ID);
        return question;
    } else {
        return {};
    }
}

std::vector<SortQuestions::Question> SortQuestions::getAll() {
    auto result = std::vector<SortQuestions::Question>{};
    utils::transform(MainImpl::getAll(session), result, fromDB);
    for (auto &q : result) {
        q.options = getOptions(q.ID);
    }
    return result;
}

std::vector<SortQuestions::Question::Option> SortQuestions::getOptions(std::int32_t questionID) {
    auto getOptionsSql = OptionsTable::Sql::select().where(OptionsTable::QuestionID == questionID);
    auto result = std::vector<Question::Option>{};
    utils::transform(session.getResults(getOptionsSql), result, optionFromDB);
    return result;
}

void SortQuestions::remove(std::int32_t ID) {
    auto sql = MainTable::Sql::select().where(MainTable::ID == ID);
    auto dbTuple = session.getResult(sql);

    if (!dbTuple) {
        throw InvalidData{"incorrect question ID"};
    }
    if (std::get<MainTable::FieldRefCount>(dbTuple.value()).value != 0) {
        throw InUse{"question in use"};
    }
    MainImpl::remove(session, ID);
}

void SortQuestions::insert(std::vector<SortQuestions::Question> *questions) {
    assert(questions);
    for (auto &question : *questions) {
        insert(&question);
    }
}

void SortQuestions::insert(SortQuestions::Question *question) {
    assert(question);
    checkOptions(question->options);
    checkName(question->name);

    question->ID = MainImpl::insert(session, toDB(*question));
    for (auto &option : question->options) {
        insert(&option, question->ID);
    }
}

void SortQuestions::checkName(const std::string &name) {
    if (name.empty()) {
        throw InvalidData{"question name cannot be empty"};
    }

    auto sql = db::sql::Select<MainTable::FieldName>{}.where(MainTable::Name == name);
    if (session.getResult(sql)) {
        throw DuplicateName{};
    }
}

void SortQuestions::checkOptions(const std::vector<Question::Option> &options) {
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

void SortQuestions::insert(Question::Option *option, std::int32_t qID) {
    assert(option);
    option->ID = OptionsImpl::insert(session, optionToDB(*option, qID));
}

namespace {
MainTable::Sql::in_t toDB(const SortQuestions::Question &question) {
    return std::make_tuple(MainTable::FieldName{question.name},
                           MainTable::FieldQuestion{question.question});
}

SortQuestions::Question fromDB(const MainTable::Sql::out_t &question) {
    auto res = SortQuestions::Question{};
    res.ID = std::get<MainTable::FieldID>(question).value;
    res.name = std::get<MainTable::FieldName>(question).value;
    res.question = std::get<MainTable::FieldQuestion>(question).value;
    return res;
}

OptionsTable::Sql::in_t optionToDB(const SortQuestions::Question::Option &option,
                                   std::int32_t questionID) {
    return std::make_tuple(OptionsTable::FieldQuestionID{questionID},
                           OptionsTable::FieldText{option.text});
}

SortQuestions::Question::Option optionFromDB(const OptionsTable::Sql::out_t &option) {
    auto res = SortQuestions::Question::Option{};
    res.ID = std::get<OptionsTable::FieldID>(option).value;
    res.text = std::get<OptionsTable::FieldText>(option).value;
    return res;
}
}
}