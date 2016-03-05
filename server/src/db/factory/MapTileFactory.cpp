#include <boost/lexical_cast.hpp>

#include "MapTileFactory.h"

namespace db {
namespace factory {

MapTileFactory::Product MapTileFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    MapTile res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.filename));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.floor));
    assert(boost::conversion::try_lexical_convert(raw[2].value(), res.zoomLevel));
    assert(boost::conversion::try_lexical_convert(raw[3].value(), res.row));
    assert(boost::conversion::try_lexical_convert(raw[4].value(), res.col));
    return res;
}

const std::vector<std::string> &MapTileFactory::fieldsOrder() noexcept {
    using namespace db::info::map_tiles;
    static const std::vector<std::string> order = {
        colFilename, colFloor, colZoomLevel, colRow, colColumn};
    return order;
}
}
}