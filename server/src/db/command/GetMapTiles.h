#ifndef DB_CMD__GET_MAP_TILES__H
#define DB_CMD__GET_MAP_TILES__H

#include <boost/optional.hpp>

#include "db/DatabaseSession.h"
#include "db/struct/MapTile.h"

namespace db {
    namespace cmd {
        class GetMapTiles {
        public:
            GetMapTiles() = default;
            GetMapTiles(std::int32_t floor);
            GetMapTiles(std::int32_t floor, std::int32_t zoomLevel);
            ~GetMapTiles() = default;

            void operator()(DatabaseSession &session);
            const std::vector<MapTile> &getResult() const;

        private:
            boost::optional<std::int32_t> floor;
            boost::optional<std::int32_t> zoomLevel;
            std::vector<MapTile> result;

            std::string createQuery() const;
        };
    }
}

#endif