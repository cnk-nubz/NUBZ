#ifndef DB_SQL__INSERT__H
#define DB_SQL__INSERT__H

#include <cassert>
#include <string>

#include <boost/format.hpp>

#include <db/table/Column.h>

#include "utils.h"

namespace db {
namespace sql {

template <class Subclass, class... Fields>
class _Insert {
    template <class Subclass2, class... Fields2>
    friend class _Insert;

    static_assert(utils::same_table<Fields...>::value, "all fields should be from the same table");

public:
    template <class Subclass2>
    _Insert(_Insert<Subclass2, Fields...> that) : vals(std::move(that.vals)) {
    }

    _Insert() = default;

    template <class... Values>
    Subclass &values(const std::tuple<Fields...> &vals) {
        auto strVals = tupleToStrings(vals, std::index_sequence_for<Values...>());
        this->vals.push_back("(" + utils::asSqlList(strVals) + ")");
        return *static_cast<Subclass *>(this);
    }

protected:
    std::string strPrefix() const {
        assert(vals.size() > 0 && "you should call .values() at least once");
        boost::format stmt("INSERT INTO %1%\n(%2%)\nVALUES\n%3%");
        stmt % utils::getTableName<Fields...>();
        stmt % utils::asSqlList({utils::getColumnName<Fields>()...});
        stmt % utils::asSqlList(vals, ",\n");
        return stmt.str();
    }

private:
    template <class Tuple, std::size_t... Idxs>
    static std::vector<std::string> tupleToStrings(const Tuple &tuple,
                                                   std::index_sequence<Idxs...>) {
        return {translation::sqlVal(std::get<Idxs>(tuple).value)...};
    }

    std::vector<std::string> vals;
};

template <class, class...>
class ReturningInsert;

template <class... Fields>
class Insert : public _Insert<Insert<Fields...>, Fields...> {
public:
    template <class RetColumn>
    auto returning(RetColumn) const {
        using RetField = typename RetColumn::field_type;
        static_assert(utils::same_table<RetField, Fields...>::value,
                      "returned field should be from the same table as inserted fields");
        return ReturningInsert<RetField, Fields...>{
            *static_cast<const _Insert<Insert<Fields...>, Fields...> *>(this)};
    }

    std::string str() const {
        return this->strPrefix();
    }
};

template <class RetField, class... Fields>
class ReturningInsert : public _Insert<ReturningInsert<RetField, Fields...>, Fields...> {
    static_assert(utils::same_table<RetField, Fields...>::value,
                  "returned field should be from the same table as inserted fields");

public:
    using return_type = std::tuple<RetField>;

    template <class _Insert2>
    ReturningInsert(_Insert2 &&core)
        : _Insert<ReturningInsert<RetField, Fields...>, Fields...>{std::forward<_Insert2>(core)} {
    }

    std::string str() const {
        boost::format stmt("%1%\nRETURNING %2%");
        stmt % this->strPrefix();
        stmt % utils::getColumnName<RetField>();
        return stmt.str();
    }
};
}
}

#endif