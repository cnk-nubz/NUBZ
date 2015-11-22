#ifndef DB_CMD__GET_VERSION__H
#define DB_CMD__GET_VERSION__H

#include "db/DatabaseSession.h"
#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        class GetVersion {
        public:
            GetVersion(db::info::versions::element_type elementType);
            ~GetVersion() = default;

            void operator()(DatabaseSession &session);
            std::int32_t getResult() const;

        private:
            db::info::versions::element_type elementType;
            std::int32_t result;

            std::string createQuery() const;
        };
    }
}

#endif