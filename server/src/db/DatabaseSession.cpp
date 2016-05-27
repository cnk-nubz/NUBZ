#include "DatabaseSession.h"

namespace db {

DatabaseSession::DatabaseSession(pqxx::work &work) : work(work) {
}

void DatabaseSession::execute(const std::string &sqlStmt) {
    work.exec(sqlStmt);
}

boost::optional<DatabaseSession::Row> DatabaseSession::getResult(const std::string &sqlQuery) {
    pqxx::result res = work.exec(sqlQuery);
    if (res.empty()) {
        return {};
    } else {
        return translate(res[0]);
    }
}

std::vector<DatabaseSession::Row> DatabaseSession::getResults(const std::string &sqlQuery) {
    pqxx::result res = work.exec(sqlQuery);
    std::vector<Row> translated;
    for (const auto &row : res) {
        translated.push_back(translate(row));
    }
    return translated;
}

DatabaseSession::Row DatabaseSession::translate(const pqxx::tuple &row) {
    Row translated;
    for (const auto &field : row) {
        translated.push_back(translate(field));
    }
    return translated;
}

DatabaseSession::Field DatabaseSession::translate(const pqxx::field &field) {
    std::string str;
    if (field.to(str)) {
        return str;
    }
    return {};
}
}