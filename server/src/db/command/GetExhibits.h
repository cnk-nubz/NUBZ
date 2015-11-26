#ifndef DB_CMD__GET_EXHIBITS__H
#define DB_CMD__GET_EXHIBITS__H

#include "db/DatabaseSession.h"
#include "db/struct/Exhibit.h"

namespace db {
    namespace cmd {
        class GetExhibits {
        public:
            GetExhibits();
            GetExhibits(std::int32_t minVersion);
            ~GetExhibits() = default;

            void operator()(DatabaseSession &session);
            const std::vector<Exhibit> &getResult() const;

        private:
            std::int32_t minVersion;
            std::vector<Exhibit> result;

            std::string createQuery() const;
            std::string createMaxVersionQuery() const;
        };
    }
}

#endif