#ifndef DB__DATABASE_SESSION__H
#define DB__DATABASE_SESSION__H

#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <pqxx/pqxx>

namespace db {

class DatabaseSession {
public:
    using Field = boost::optional<std::string>;
    using Row = std::vector<Field>;

    DatabaseSession(pqxx::work &work);

    void execute(const std::string &sqlStmt);

    // first result
    Row getResult(const std::string &sqlQuery);

    // all results
    std::vector<Row> getResults(const std::string &sqlQuery);

    // first result
    template <class T>
    typename T::Product getResult(const std::string &sqlQuery);

    // all results
    template <class T>
    std::vector<typename T::Product> getResults(const std::string &sqlQuery);

private:
    pqxx::work &work;

    Row translate(const pqxx::tuple &row) const;
    Field translate(const pqxx::field &field) const;
};

template <class T>
typename T::Product DatabaseSession::getResult(const std::string &sqlQuery) {
    return T::create(getResult(sqlQuery));
}

template <class T>
std::vector<typename T::Product> DatabaseSession::getResults(const std::string &sqlQuery) {
    std::vector<typename T::Product> res;
    for (const auto &row : getResults(sqlQuery)) {
        res.push_back(std::move(T::create(row)));
    }
    return res;
}
}

#endif