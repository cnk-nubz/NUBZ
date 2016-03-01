#ifndef DB__DATABASE_SESSION__H
#define DB__DATABASE_SESSION__H

#include <string>
#include <vector>
#include <tuple>
#include <utility>

#include <boost/optional.hpp>
#include <pqxx/pqxx>

#include <utils/fp_algorithm.h>

#include <db/table/Value.h>

namespace db {

class DatabaseSession {
public:
    using Field = boost::optional<std::string>;
    using Row = std::vector<Field>;

    DatabaseSession(pqxx::work &work);

    // raw commands
    void execute(const std::string &sqlStmt);

    Row getResult(const std::string &sqlQuery);

    std::vector<Row> getResults(const std::string &sqlQuery);

    // commands parametrized with factory
    template <class T>
    typename T::Product getResult(const std::string &sqlQuery) {
        return T::create(getResult(sqlQuery));
    }

    template <class T>
    std::vector<typename T::Product> getResults(const std::string &sqlQuery) {
        std::vector<typename T::Product> res;
        for (const auto &row : getResults(sqlQuery)) {
            res.emplace_back(T::create(row));
        }
        return res;
    }

    // commands using db::sql
    template <class Sql>
    void execute(const Sql &sql) {
        execute(sql.str());
    }

    // returns optional instead of raising error in case of no result (as previous versions do)
    template <class Sql>
    boost::optional<typename Sql::return_type> getResult(const Sql &sql) {
        auto res = getResults(sql);
        if (!res.empty()) {
            return res.front();
        } else {
            return {};
        }
    }

    template <class Sql>
    std::vector<typename Sql::return_type> getResults(const Sql &sql) {
        using return_t = typename Sql::return_type;

        auto rows = getResults(sql.str());
        std::vector<return_t> result;
        utils::transform(rows, result, translate<return_t>);
        return result;
    }

private:
    template <class Tuple>
    static Tuple translate(const std::vector<Field> &vec) {
        return translateImpl<Tuple>(vec, std::make_index_sequence<std::tuple_size<Tuple>::value>());
    }

    template <class Tuple, std::size_t... Idxs>
    static Tuple translateImpl(const std::vector<Field> &vec, std::index_sequence<Idxs...>) {
        return std::make_tuple(
            db::table::ValueFactory<std::tuple_element_t<Idxs, Tuple>>::create(vec[Idxs])...);
    }

    static DatabaseSession::Row translate(const pqxx::tuple &row);
    static DatabaseSession::Field translate(const pqxx::field &field);

    pqxx::work &work;
};
}

#endif