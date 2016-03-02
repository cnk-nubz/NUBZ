#ifndef DB_FACTORY__MAP_TILES_INFO_FACTORY__H
#define DB_FACTORY__MAP_TILES_INFO_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/db_info.h>
#include <db/struct/MapTilesInfo.h>

namespace db {
namespace factory {

struct MapTilesInfoFactory {
    using Product = MapTilesInfo;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif