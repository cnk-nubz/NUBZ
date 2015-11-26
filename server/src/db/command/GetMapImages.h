#ifndef DB_CMD__GET_MAP_IMAGES__H
#define DB_CMD__GET_MAP_IMAGES__H

#include "db/DatabaseSession.h"
#include "db/struct/MapImage.h"

namespace db {
    namespace cmd {
        class GetMapImages {
        public:
            GetMapImages();
            GetMapImages(std::int32_t minVersion);
            ~GetMapImages() = default;

            void operator()(DatabaseSession &session);
            const std::vector<MapImage> &getResult() const;

        private:
            std::int32_t minVersion;
            std::vector<MapImage> result;

            std::string createQuery() const;
        };
    }
}

#endif