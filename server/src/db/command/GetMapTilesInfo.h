#ifndef DB_CMD__GET_MAP_TILES_INFO__H
#define DB_CMD__GET_MAP_TILES_INFO__H

#include <db/struct/MapTilesInfo.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetMapTilesInfo {
public:
    GetMapTilesInfo(std::int32_t floor);

    const std::vector<MapTilesInfo> operator()(DatabaseSession &session);
    const std::vector<MapTilesInfo> &getResult() const;

private:
    std::int32_t floor;
    std::vector<MapTilesInfo> result;

    std::string createQuery() const;
};
}
}

#endif