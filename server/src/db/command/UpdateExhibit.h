#ifndef DB_CMD__UPDATE_EXHIBIT__H
#define DB_CMD__UPDATE_EXHIBIT__H

#include <boost/optional.hpp>

#include "db/DatabaseSession.h"
#include "db/struct/Exhibit.h"

namespace db {
    namespace cmd {
        class UpdateExhibit {
        public:
            UpdateExhibit(const Exhibit &exhibit);
            ~UpdateExhibit() = default;

            void operator()(DatabaseSession &session);

        private:
            const Exhibit exhibit;

            std::string createUpdate() const;
        };
    }
}

#endif