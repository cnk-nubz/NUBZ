#ifndef DB_CMD__REMOVE_TILES_INFO__H
#define DB_CMD__REMOVE_TILES_INFO__H

#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class RemoveMapTilesInfo {
public:
    RemoveMapTilesInfo(std::int32_t floor);

    void operator()(DatabaseSession &session);

private:
    std::int32_t floor;

    std::string createDelete() const;
};
}
}

#endif