#ifndef REPOSITORY__DEFAULT_REPO__H
#define REPOSITORY__DEFAULT_REPO__H

#include <boost/optional.hpp>

#include <utils/fp_algorithm.h>

#include <db/DatabaseSession.h>

namespace repository {
namespace detail {

template <class DBTable>
struct _DefaultRepo {
    using in_t = typename DBTable::Sql::in_t;
    using out_t = typename DBTable::Sql::out_t;

    static std::vector<out_t> getAll(db::DatabaseSession &session) {
        auto sql = DBTable::Sql::select();
        return session.getResults(sql);
    }

    static void removeAll(db::DatabaseSession &session) {
        auto sql = DBTable::Sql::del();
        session.execute(sql);
    }
};

template <class DBTable>
struct DefaultRepo : _DefaultRepo<DBTable> {
    using typename _DefaultRepo<DBTable>::in_t;

    static void insert(db::DatabaseSession &session, const in_t &val) {
        insertAll(session, {val});
    }

    static void insertAll(db::DatabaseSession &session, const std::vector<in_t> &vals) {
        auto sql = DBTable::Sql::insert();
        for (const auto &v : vals) {
            sql.values(v);
        }
        session.execute(sql);
    }
};

template <class DBTable>
struct DefaultRepoWithID : _DefaultRepo<DBTable> {
    using typename _DefaultRepo<DBTable>::in_t;
    using typename _DefaultRepo<DBTable>::out_t;
    using field_id_t = typename DBTable::FieldID;
    using id_t = typename field_id_t::type;

    static std::vector<id_t> getAllIDs(db::DatabaseSession &session) {
        auto sql = db::sql::Select<field_id_t>{};
        auto result = std::vector<id_t>{};
        utils::transform(session.getResults(sql), result, [](auto &dbTuple) {
            return std::get<field_id_t>(dbTuple).value;
        });
        return result;
    }

    static boost::optional<out_t> get(db::DatabaseSession &session, id_t ID) {
        auto sql = DBTable::Sql::select();
        sql.where(DBTable::ID == ID);
        return session.getResult(sql);
    }

    static void remove(db::DatabaseSession &session, id_t ID) {
        auto sql = DBTable::Sql::del().where(DBTable::ID == ID);
        session.execute(sql);
    }

    // returns id of inserted row
    static id_t insert(db::DatabaseSession &session, const in_t &val) {
        return insertAll(session, {val}).front();
    }

    // returns ids of inserted rows
    static std::vector<id_t> insertAll(db::DatabaseSession &session,
                                       const std::vector<in_t> &vals) {
        auto sql = DBTable::Sql::insert().returning(DBTable::ID);
        for (const auto &val : vals) {
            sql.values(val);
        }

        auto result = std::vector<id_t>{};
        auto ids = session.getResults(sql);
        for (const auto insertedId : ids) {
            result.push_back(std::get<field_id_t>(insertedId).value);
        }
        return result;
    }
};
}
}

#endif