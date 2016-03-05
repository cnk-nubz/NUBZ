#include <boost/lexical_cast.hpp>

#include "ExhibitFactory.h"

namespace db {
namespace factory {

ExhibitFactory::Product ExhibitFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.begin() + 3, [](const auto &e) { return (bool)e; }));

    Exhibit res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.name));
    assert(boost::conversion::try_lexical_convert(raw[2].value(), res.version));
    res.frame = createFrame(raw);

    return res;
}

boost::optional<MapFrame> ExhibitFactory::createFrame(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    bool exist = raw[3].is_initialized();
    for (std::size_t i = 4; i < raw.size(); i++) {
        assert(exist == raw[i].is_initialized());
    }
    if (!exist) {
        return {};
    }

    MapFrame res;
    assert(boost::conversion::try_lexical_convert(raw[3].value(), res.x));
    assert(boost::conversion::try_lexical_convert(raw[4].value(), res.y));
    assert(boost::conversion::try_lexical_convert(raw[5].value(), res.width));
    assert(boost::conversion::try_lexical_convert(raw[6].value(), res.height));
    assert(boost::conversion::try_lexical_convert(raw[7].value(), res.floor));
    return res;
}

const std::vector<std::string> &ExhibitFactory::fieldsOrder() noexcept {
    using namespace db::info::exhibits;
    static const std::vector<std::string> order = {colId,
                                                   colName,
                                                   colVersion,
                                                   colMapFrameX,
                                                   colMapFrameY,
                                                   colMapFrameWidth,
                                                   colMapFrameHeight,
                                                   colMapFloor};
    return order;
}
}
}