#ifndef DB_CMD__DATABASE_COMMAND__H
#define DB_CMD__DATABASE_COMMAND__H

#include <pqxx/pqxx>

#include "db/DatabaseSession.h"

namespace db {
    namespace cmd {
        class DatabaseCommand {
        public:
            virtual ~DatabaseCommand() = default;

            virtual void perform(DatabaseSession &session) = 0;
        };
    }
}

#endif