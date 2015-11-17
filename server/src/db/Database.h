#ifndef DB__Database__H
#define DB__Database__H

#include <thread>
#include <memory>

#include <pqxx/pqxx>

#include "command/DatabaseCommand.h"

namespace db {
    class Database {
    public:
        Database(const std::string &user, const std::string &db, const std::string &host,
                 std::uint16_t port);
        ~Database() = default;
        Database(const Database &) = delete;
        Database(Database &&) = delete;
        Database &operator=(const Database &) = delete;
        Database &operator=(Database &&) = delete;

        // thread-safe
        void execute(cmd::DatabaseCommand *cmd);

    private:
        std::unique_ptr<pqxx::lazyconnection> connection;
        std::mutex cmdMutex;
    };
}

#endif