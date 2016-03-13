#include <boost/format.hpp>

#include "Database.h"

namespace db {

Database::Database(const std::string &user, const std::string &db, const std::string &host,
                   std::uint16_t port) {
    boost::format initMsg("user=%1% dbname=%2% host=%3% port=%4%");
    initMsg % user;
    initMsg % db;
    initMsg % host;
    initMsg % port;

    connection.reset(new pqxx::lazyconnection(initMsg.str()));
}

DatabaseException::DatabaseException(const pqxx::failure &e) : std::runtime_error(e.what()) {
}

Database::WorkGuard::WorkGuard(pqxx::lazyconnection &lc) : canceled(false), work(lc) {
}

Database::WorkGuard::~WorkGuard() {
    if (!canceled) {
        work.commit();
    }
}

pqxx::work &Database::WorkGuard::getWork() {
    return work;
}

void Database::WorkGuard::cancel() {
    canceled = true;
}
}