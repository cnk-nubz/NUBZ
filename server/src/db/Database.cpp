#include <boost/format.hpp>

#include "external/easylogging++.h"
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

    void Database::execute(cmd::DatabaseCommand *cmd) {
        std::lock_guard<std::mutex> lock(cmdMutex);
        pqxx::work work(*connection);
        DatabaseSession session(work);

        try {
            cmd->perform(session);
            work.commit();
        } catch (pqxx::broken_connection &e) {
            LOG(DEBUG) << e.what();
            throw;
        } catch (pqxx::sql_error &e) {
            LOG(DEBUG) << e.what();
            throw;
        } catch (...) {
            throw;
        }
    }
}