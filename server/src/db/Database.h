#ifndef DB__Database__H
#define DB__Database__H

#include <thread>
#include <memory>
#include <mutex>

#include <pqxx/pqxx>

#include "external/easylogging++.h"

#include "DatabaseSession.h"

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
        template <class CMD>
        void execute(CMD &&cmd);

    private:
        std::unique_ptr<pqxx::lazyconnection> connection;
        std::mutex cmdMutex;
    };

    struct DatabaseException : public std::runtime_error {
        DatabaseException(const pqxx::broken_connection &e);
        DatabaseException(const pqxx::sql_error &e);
    };

    template <class CMD>
    void Database::execute(CMD &&cmd) {
        std::lock_guard<std::mutex> lock(cmdMutex);
        pqxx::work work(*connection);
        DatabaseSession session(work);

        try {
            cmd(session);
            work.commit();
        } catch (pqxx::broken_connection &e) {
            LOG(DEBUG) << e.what();
            throw DatabaseException(e);
        } catch (pqxx::sql_error &e) {
            LOG(DEBUG) << e.what();
            throw DatabaseException(e);
        }
    }
}

#endif