#ifndef DB_CMD__SAVE_MAP_IMAGE__H
#define DB_CMD__SAVE_MAP_IMAGE__H

#include "db/DatabaseSession.h"
#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        class SaveMapImage {
        public:
            SaveMapImage(const MapImage &mapImage);
            ~SaveMapImage() = default;

            void operator()(DatabaseSession &session);

        private:
            const MapImage mapImage;

            std::string createQuery() const;
            std::string createInsert() const;
            std::string createUpdate() const;
        };
    }
}

#endif