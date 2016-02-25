#include <boost/lexical_cast.hpp>

#include "SortQuestionFactory.h"

namespace db {
namespace factory {

SortQuestionFactory::Product SortQuestionFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    SortQuestion res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    res.name = raw[1].value();
    res.question = raw[2].value();
    return res;
}

const std::vector<std::string> &SortQuestionFactory::fieldsOrder() noexcept {
    using namespace db::info::sort_questions;
    static const std::vector<std::string> order = {colId, colName, colQuestion};
    return order;
}
}
}