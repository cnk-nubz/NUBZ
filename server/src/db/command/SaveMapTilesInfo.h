#ifndef DB_CMD__SAVE_MAP_TILES_INFO__H
#define DB_CMD__SAVE_MAP_TILES_INFO__H

#include "db/DatabaseSession.h"
#include "db/struct/MapTilesInfo.h"

namespace db {
    namespace cmd {
        class SaveMapTilesInfo {
        public:
            SaveMapTilesInfo(const MapTilesInfo &mapTileInfo);
            ~SaveMapTilesInfo() = default;

            void operator()(DatabaseSession &session);

        private:
            const MapTilesInfo mapTileInfo;

            std::string createInsert() const;
        };
    }
}

#endif