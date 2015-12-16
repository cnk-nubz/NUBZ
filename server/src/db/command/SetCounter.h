#ifndef DB_CMD__SET_COUNTER__H
#define DB_CMD__SET_COUNTER__H

#include "db/DatabaseSession.h"
#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace cmd {
        class SetCounter {
        public:
            SetCounter(db::info::counters::element_type elementType, std::int32_t newValue);
            ~SetCounter() = default;

            void operator()(DatabaseSession &session);

        private:
            const db::info::counters::element_type elementType;
            const std::int32_t newValue;

            std::string createUpdate() const;
        };
    }
}

#endif