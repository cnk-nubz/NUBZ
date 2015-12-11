#ifndef DB_CMD__SAVE_RAW_REPORT__H
#define DB_CMD__SAVE_RAW_REPORT__H

#include "db/DatabaseSession.h"
#include "db/struct/RawReport.h"

namespace db {
    namespace cmd {
        class SaveRawReport {
        public:
            SaveRawReport(const db::RawReport &report);
            ~SaveRawReport() = default;

            void operator()(DatabaseSession &session);

        private:
            const db::RawReport report;

            std::string createInsert() const;
        };
    }
}

#endif