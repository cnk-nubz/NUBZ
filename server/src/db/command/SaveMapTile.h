#ifndef DB_CMD__SAVE_MAP_TILE__H
#define DB_CMD__SAVE_MAP_TILE__H

#include <db/DatabaseSession.h>
#include <db/struct/MapTile.h>

namespace db {
namespace cmd {

class SaveMapTile {
public:
    SaveMapTile(const MapTile &MapTile);

    void operator()(DatabaseSession &session);

private:
    const MapTile mapTile;

    std::string createInsert() const;
};
}
}

#endif