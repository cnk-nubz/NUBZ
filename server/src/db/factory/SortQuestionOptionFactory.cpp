#include <boost/lexical_cast.hpp>

#include "SortQuestionOptionFactory.h"

namespace db {
namespace factory {

SortQuestionOptionFactory::Product SortQuestionOptionFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    SortQuestionOption res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.questionId));
    res.text = raw[2].value();
    return res;
}

const std::vector<std::string> &SortQuestionOptionFactory::fieldsOrder() noexcept {
    using namespace db::info::sort_question_options;
    static const std::vector<std::string> order = {colId, colQuestionId, colText};
    return order;
}
}
}