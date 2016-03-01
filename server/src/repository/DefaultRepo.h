#ifndef REPOSITORY__DEFAULT_REPO__H
#define REPOSITORY__DEFAULT_REPO__H

#include <boost/optional.hpp>

#include <utils/fp_algorithm.h>

#include <db/DatabaseSession.h>

namespace repository {
namespace detail {

template <class DBTable>
struct _DefaultRepo {
    using val_type = typename DBTable::Row;

    static std::vector<val_type> getAll(db::DatabaseSession &session) {
        auto sql = DBTable::select();
        std::vector<val_type> result;
        utils::transform(session.getResults(sql), result, DBTable::RowFactory::fromDB);
        return result;
    }

    static void removeAll(db::DatabaseSession &session) {
        auto sql = DBTable::del();
        session.execute(sql);
    }
};

template <class DBTable>
struct DefaultRepo : _DefaultRepo<DBTable> {
    using typename _DefaultRepo<DBTable>::val_type;

    static void insert(db::DatabaseSession &session, const val_type &val) {
        insertAll(session, {val});
    }

    static void insertAll(db::DatabaseSession &session, const std::vector<val_type> &vals) {
        auto sql = DBTable::insert();
        for (const auto &v : vals) {
            sql.insert(DBTable::RowFactory::toDB(v));
        }
        session.execute(sql);
    }
};

template <class DBTable>
struct DefaultRepoWithID : _DefaultRepo<DBTable> {
    using typename _DefaultRepo<DBTable>::val_type;
    using id_col_type = typename DBTable::ColumnId;
    using id_type = typename id_col_type::value_type::type;

    static boost::optional<val_type> get(db::DatabaseSession &session, id_type ID) {
        auto sql = DBTable::select();
        sql.where(DBTable::colId == ID);

        if (auto row = session.getResult(sql)) {
            return DBTable::RowFactory::fromDB(row.value());
        } else {
            return {};
        }
    }

    static void remove(db::DatabaseSession &session, id_type ID) {
        auto sql = DBTable::del().where(DBTable::colId == ID);
        session.execute(sql);
    }

    // returns id of inserted row
    static id_type insert(db::DatabaseSession &session, const val_type &val) {
        return insertAll(session, {val}).front();
    }

    // returns ids of inserted rows
    static std::vector<id_type> insertAll(db::DatabaseSession &session,
                                          const std::vector<val_type> &vals) {
        auto sql = DBTable::insert().returning(DBTable::colId);
        for (const auto &val : vals) {
            sql.values(DBTable::RowFactory::toDB(val));
        }

        auto ids = session.getResults(sql);
        std::vector<id_type> result;
        for (const auto insertedId : ids) {
            result.push_back(std::get<typename id_col_type::value_type>(insertedId).value);
        }
        return result;
    }
};
}
}

#endif