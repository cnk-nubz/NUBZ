#ifndef DB_FACTORY__MAP_TILE_FACTORY__H
#define DB_FACTORY__MAP_TILE_FACTORY__H

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include <db/db_info.h>
#include <db/struct/MapTile.h>

namespace db {
namespace factory {

struct MapTileFactory {
    using Product = MapTile;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif