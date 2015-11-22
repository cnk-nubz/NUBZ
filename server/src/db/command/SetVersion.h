#ifndef DB_CMD__SET_VERSION__H
#define DB_CMD__SET_VERSION__H

#include "db/DatabaseSession.h"
#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        class SetVersion {
        public:
            SetVersion(db::info::versions::element_type elementType, std::int32_t newVersion);
            ~SetVersion() = default;

            void operator()(DatabaseSession &session);

        private:
            db::info::versions::element_type elementType;
            std::int32_t newVersion;

            std::string createUpdate() const;
        };
    }
}

#endif