#ifndef DB_CMD__GET_MAP_TILES__H
#define DB_CMD__GET_MAP_TILES__H

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/MapTile.h>

namespace db {
namespace cmd {

class GetMapTiles {
public:
    std::vector<MapTile> operator()(DatabaseSession &session);
    const std::vector<MapTile> &getResult() const;

    boost::optional<std::int32_t> floor;
    boost::optional<std::int32_t> zoomLevel;

private:
    std::vector<MapTile> result;

    std::string createQuery() const;
};
}
}

#endif