#include <utils/fp_algorithm.h>

#include <db/table/SimpleQuestions.h>

#include "DefaultRepo.h"
#include "SimpleQuestions.h"

namespace repository {

using Table = db::table::SimpleQuestions;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
Table::Row toDB(const SimpleQuestions::Question &question);
SimpleQuestions::Question fromDB(const Table::Row &question);
}

SimpleQuestions::SimpleQuestions(db::DatabaseSession &session) : session(session) {
}

boost::optional<SimpleQuestions::Question> SimpleQuestions::get(std::int32_t ID) {
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
    Impl::remove(session, ID);
}

void SimpleQuestions::removeAll() {
    Impl::removeAll(session);
}

void SimpleQuestions::insert(std::vector<SimpleQuestions::Question> *questions) {
    assert(questions);
    for (auto &question : *questions) {
        insert(&question);
    }
}

void SimpleQuestions::insert(SimpleQuestions::Question *question) {
    assert(question);
    question->ID = Impl::insert(session, toDB(*question));
}

namespace {
Table::Row toDB(const SimpleQuestions::Question &question) {
    auto res = Table::Row{};
    res.ID = question.ID;
    res.name = question.name;
    res.question = question.question;
    res.numberAnswer = question.numberAnswer;
    return res;
}

SimpleQuestions::Question fromDB(const Table::Row &question) {
    auto res = SimpleQuestions::Question{};
    res.ID = question.ID;
    res.name = question.name;
    res.question = question.question;
    res.numberAnswer = question.numberAnswer;
    return res;
}
}
}