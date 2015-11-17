#ifndef DB_CMD__GET_MAP_IMAGES__H
#define DB_CMD__GET_MAP_IMAGES__H

#include "DatabaseCommand.h"
#include "db/struct/MapImage.h"

namespace db {
    namespace cmd {
        class GetMapImages : public DatabaseCommand {
        public:
            GetMapImages();
            GetMapImages(std::int32_t minVersion);
            virtual ~GetMapImages() = default;

            GetMapImages(const GetMapImages &) = delete;
            GetMapImages(GetMapImages &&) = default;
            GetMapImages &operator=(const GetMapImages &) = delete;
            GetMapImages &operator=(GetMapImages &&) = default;

            virtual void perform(DatabaseSession &session);
            const std::vector<MapImage> &getResult() const;

        private:
            std::int32_t minVersion;
            std::vector<MapImage> result;

            std::string createQuery() const;
            std::string createMaxVersionQuery() const;
        };
    }
}

#endif