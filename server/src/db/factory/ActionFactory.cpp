#include <boost/lexical_cast.hpp>

#include "ActionFactory.h"
#include "SingleFieldFactory.h"

namespace db {
namespace factory {

ActionFactory::Product ActionFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(raw[0]);
    assert(raw[1]);

    Action res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    res.text = raw[1].value();
    return res;
}

const std::vector<std::string> &ActionFactory::fieldsOrder() noexcept {
    using namespace db::info::actions;
    static const std::vector<std::string> order = {colId, colText};
    return order;
}
}
}