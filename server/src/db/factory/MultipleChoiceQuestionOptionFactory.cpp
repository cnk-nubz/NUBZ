#include <boost/lexical_cast.hpp>

#include "MultipleChoiceQuestionOptionFactory.h"

namespace db {
namespace factory {

MultipleChoiceQuestionOptionFactory::Product MultipleChoiceQuestionOptionFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    MultipleChoiceQuestionOption res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.questionId));
    res.text = raw[2].value();
    return res;
}

const std::vector<std::string> &MultipleChoiceQuestionOptionFactory::fieldsOrder() noexcept {
    using namespace db::info::multiple_choice_question_options;
    static const std::vector<std::string> order = {colId, colQuestionId, colText};
    return order;
}
}
}