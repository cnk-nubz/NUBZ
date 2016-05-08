#include <utils/fp_algorithm.h>

#include <db/table/SimpleQuestions.h>

#include "DefaultRepo.h"
#include "SimpleQuestions.h"
#include "error/DuplicateName.h"
#include "error/InUse.h"
#include "error/InvalidData.h"

namespace repository {

using Table = db::table::SimpleQuestions;
using Impl = repository::detail::DefaultRepoIDRefCount<Table>;

namespace {
Table::Sql::in_t toDB(const SimpleQuestions::Question &question);
SimpleQuestions::Question fromDB(const Table::Sql::out_t &question);
}

SimpleQuestions::SimpleQuestions(db::DatabaseSession &session) : session(session) {
}

void SimpleQuestions::incReferenceCount(std::int32_t ID) {
    Impl::incRefCount(session, ID);
}

void SimpleQuestions::decReferenceCount(std::int32_t ID) {
    Impl::decRefCount(session, ID);
}

SimpleQuestions::Question SimpleQuestions::get(std::int32_t ID) {
    if (auto res = getOpt(ID)) {
        return res.value();
    } else {
        throw InvalidData{"incorrect question ID"};
    }
}

boost::optional<SimpleQuestions::Question> SimpleQuestions::getOpt(std::int32_t ID) {
    if (auto res = Impl::get(session, ID)) {
        return fromDB(res.value());
    } else {
        return {};
    }
}

std::vector<SimpleQuestions::Question> SimpleQuestions::getAll() {
    auto result = std::vector<SimpleQuestions::Question>{};
    utils::transform(Impl::getAll(session), result, fromDB);
    return result;
}

void SimpleQuestions::remove(std::int32_t ID) {
    auto sql = Table::Sql::select().where(Table::ID == ID);
    auto dbTuple = session.getResult(sql);

    if (!dbTuple) {
        throw InvalidData{"incorrect question ID"};
    }
    if (std::get<Table::FieldRefCount>(dbTuple.value()).value != 0) {
        throw InUse{"question in use"};
    }
    Impl::remove(session, ID);
}

void SimpleQuestions::insert(std::vector<SimpleQuestions::Question> *questions) {
    assert(questions);
    for (auto &question : *questions) {
        insert(&question);
    }
}

void SimpleQuestions::insert(SimpleQuestions::Question *question) {
    assert(question);
    checkName(question->name);
    question->ID = Impl::insert(session, toDB(*question));
}

void SimpleQuestions::checkName(const std::string &name) {
    if (name.empty()) {
        throw InvalidData{"question name cannot be empty"};
    }

    auto sql = db::sql::Select<Table::FieldName>{}.where(Table::Name == name);
    if (session.getResult(sql)) {
        throw DuplicateName{};
    }
}

namespace {
Table::Sql::in_t toDB(const SimpleQuestions::Question &question) {
    return std::make_tuple(Table::FieldName{question.name},
                           Table::FieldQuestion{question.question},
                           Table::FieldNumberAnswer{question.numberAnswer});
}

SimpleQuestions::Question fromDB(const Table::Sql::out_t &question) {
    auto res = SimpleQuestions::Question{};
    res.ID = std::get<Table::FieldID>(question).value;
    res.name = std::get<Table::FieldName>(question).value;
    res.question = std::get<Table::FieldQuestion>(question).value;
    res.numberAnswer = std::get<Table::FieldNumberAnswer>(question).value;
    return res;
}
}
}