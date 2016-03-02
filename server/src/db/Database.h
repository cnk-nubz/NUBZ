#ifndef DB__Database__H
#define DB__Database__H

#include <memory>
#include <mutex>
#include <thread>

#include <pqxx/pqxx>

#include <utils/log.h>

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
    template <class Cmd>
    std::result_of_t<Cmd(DatabaseSession &)> execute(Cmd &&cmd);

private:
    class WorkGuard {
    public:
        WorkGuard(pqxx::lazyconnection &lc);
        ~WorkGuard();

        pqxx::work &getWork();
        void cancel();

    private:
        bool canceled;
        pqxx::work work;
    };

    std::unique_ptr<pqxx::lazyconnection> connection;
    std::mutex cmdMutex;
};

struct DatabaseException : public std::runtime_error {
    DatabaseException(const pqxx::broken_connection &e);
    DatabaseException(const pqxx::sql_error &e);
};

template <class Cmd>
std::result_of_t<Cmd(DatabaseSession &)> Database::execute(Cmd &&cmd) {
    std::lock_guard<std::mutex> lock(cmdMutex);
    WorkGuard transaction(*connection);
    DatabaseSession session(transaction.getWork());

    try {
        return cmd(session);
    } catch (pqxx::broken_connection &e) {
        transaction.cancel();
        LOG(DEBUG) << e.what();
        throw DatabaseException(e);
    } catch (pqxx::sql_error &e) {
        transaction.cancel();
        LOG(DEBUG) << e.what();
        throw DatabaseException(e);
    }
}
}

#endif