#include <boost/lexical_cast.hpp>

#include "MapImageFactory.h"

namespace db {
namespace factory {

MapImageFactory::Product MapImageFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    MapImage res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.filename));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.width));
    assert(boost::conversion::try_lexical_convert(raw[2].value(), res.height));
    assert(boost::conversion::try_lexical_convert(raw[3].value(), res.version));
    assert(boost::conversion::try_lexical_convert(raw[4].value(), res.floor));
    return res;
}

const std::vector<std::string> &MapImageFactory::fieldsOrder() noexcept {
    using namespace db::info::map_images;
    static const std::vector<std::string> order = {
        colFilename, colWidth, colHeight, colVersion, colFloor};
    return order;
}
}
}