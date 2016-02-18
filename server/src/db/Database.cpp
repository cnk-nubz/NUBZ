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

DatabaseException::DatabaseException(const pqxx::broken_connection &e)
    : std::runtime_error(e.what()) {
}
DatabaseException::DatabaseException(const pqxx::sql_error &e) : std::runtime_error(e.what()) {
}
}