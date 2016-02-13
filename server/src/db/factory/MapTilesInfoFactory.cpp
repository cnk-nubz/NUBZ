#include <boost/lexical_cast.hpp>

#include "MapTilesInfoFactory.h"

namespace db {
namespace factory {

MapTilesInfoFactory::Product MapTilesInfoFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    MapTilesInfo res;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), res.floor));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), res.zoomLevel));
    assert(boost::conversion::try_lexical_convert(raw[2].value(), res.rowsCount));
    assert(boost::conversion::try_lexical_convert(raw[3].value(), res.colsCount));
    assert(boost::conversion::try_lexical_convert(raw[4].value(), res.imgWidth));
    assert(boost::conversion::try_lexical_convert(raw[5].value(), res.imgHeight));
    assert(boost::conversion::try_lexical_convert(raw[6].value(), res.tileSize));
    return res;
}

const std::vector<std::string> &MapTilesInfoFactory::fieldsOrder() noexcept {
    using namespace db::info::map_tiles_info;
    static const std::vector<std::string> order = {colFloor,
                                                   colZoomLevel,
                                                   colRowsCount,
                                                   colColumnsCount,
                                                   colImgWidth,
                                                   colImgHeight,
                                                   colTileSize};
    return order;
}
}
}
