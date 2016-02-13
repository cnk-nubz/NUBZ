#include <boost/lexical_cast.hpp>

#include "SimpleQuestionFactory.h"
#include "SingleFieldFactory.h"

namespace db {
namespace factory {

SimpleQuestionFactory::Product SimpleQuestionFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    SimpleQuestion res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    res.name = raw[1].value();
    res.question = raw[2].value();
    res.numberAnswer = SingleFieldFactory<bool>::create({raw[3]});
    return res;
}

const std::vector<std::string> &SimpleQuestionFactory::fieldsOrder() noexcept {
    using namespace db::info::simple_questions;
    static const std::vector<std::string> order = {colId, colName, colQuestion, colNumberAnswer};
    return order;
}
}
}